#include "x86_codegen.hh"


void x86CodeGenerator::MunchLabel(std::shared_ptr<IRLabel> exp)
{
    auto innerLabel = exp->label;
    auto labelAsm = std::make_shared<AssemblyLabel>(
        innerLabel->ToString() + ":", // In asm the label is followed by a ':' 
        innerLabel
    );

    EmitInstruction(labelAsm);
}

std::string x86CodeGenerator::RelationalOperatorToString(RelationalOperator op)
{
    switch (op)
    {
    case RelationalOperator::Matches:
        return "je";
    case RelationalOperator::NotMatches:
        return "jne";
    case RelationalOperator::GreaterThan:
        return "jg";
    case RelationalOperator::GreaterThanOrEqual:
        return "jge";
    case RelationalOperator::LessThan:
        return "jl";
    case RelationalOperator::LessThanOrEqual:
        return "jle";
    default:
        throw std::logic_error("Invalid Relational operator!");
    }
}


void x86CodeGenerator::MunchCjump(std::shared_ptr<IRCJump> cJumpExp)
{
    // Location of operands

    auto leftTemp = MunchExpression(cJumpExp->left);
    auto rightTemp = MunchExpression(cJumpExp->right);

    auto cmpIns = std::make_shared<AssemblyOper>(
        "cmp `s0, `s1",
        nullptr, // No destination
        AppendTempList(leftTemp, AppendTempList(rightTemp,nullptr))
    );

    EmitInstruction(cmpIns);

    std::string jumpOp = RelationalOperatorToString(cJumpExp->relop);

    auto jumpTrue = std::make_shared<AssemblyOper>(
        Helper::FormatString("%s `j0", jumpOp.c_str()),
        nullptr,
        nullptr,
        std::make_shared<LabelList>( // Both labels should be in targets
            cJumpExp->iftrue,
            std::make_shared<LabelList>(
                cJumpExp->iffalse,
                nullptr)
            )
    );

    EmitInstruction(jumpTrue);

    // Don't need to jump to false, because the next instruction is the false label, so just let it fall through
}

void x86CodeGenerator::MunchEnter(std::shared_ptr<IREnter> enterExp)
{
    // There are no destination or source values for enter
    auto asmInst = std::make_shared<AssemblyOper>(
        Helper::FormatString("enter %d, 0", enterExp->neededBytes), // ',0' is needed for syntax
        nullptr,
        nullptr
    );

    EmitInstruction(asmInst);
}

void x86CodeGenerator::MunchEvaluateExpression(std::shared_ptr<IREvaluateExpression> evaluateExp)
{
    // Evaluate expression will mainly be used for void function calls

    if(auto call = std::dynamic_pointer_cast<IRCall>(evaluateExp->exp))
    {
        // Now where the func is
        auto newTemp = MunchExpression(call->func);

        // The value of caller saved registers + return register must be given as destinations, so later parts of the compiler knows that something happens to them here
        auto callDefs = GlobalFrame::globalFrameType->GetCallDefs();

        auto asmInst = std::make_shared<AssemblyOper>(
            "call `s0",
            callDefs,
            AppendTempList(newTemp, nullptr)
        );
        EmitInstruction(asmInst);
    }
    else
    {
        // Throw error to see if I missed something
        assert(false);
    }
    
}

void x86CodeGenerator::MunchJump(std::shared_ptr<IRJump> jumpExp)
{
    // Regular jumps can only go to labels which will be referenced through an IRName object
    auto name = std::dynamic_pointer_cast<IRName>(jumpExp->exp);
    if(!name)
    {
        throw std::logic_error("IRJump received a non-label expression!");
    }

    auto labelList = std::make_shared<LabelList>(
        name->label,
        nullptr
    );

    auto asmIns = std::make_shared<AssemblyOper>(
        "jmp `j0",
        nullptr,
        nullptr,
        labelList
    );

    EmitInstruction(asmIns);


}

void x86CodeGenerator::MunchLeave(std::shared_ptr<IRLeave> exp)
{
    auto asmInst = std::make_shared<AssemblyOper>(
        "leave",
        nullptr,
        nullptr
    );

    EmitInstruction(asmInst);
}

void x86CodeGenerator::MunchMove(std::shared_ptr<IRMove> moveExp)
{
    /*
        The only valid moves are:

        Immediate → Register	mov rax, 42 [DONE]
        Memory → Register	    mov rax, [rsp+8] [DONE]
        Label → Register	    mov rax, TEST_FUNCTION_LABEL (for virtual functions) [DONE]

        Immediate → Memory	    mov [rsp+8], 42 [DONE]
        Register → Memory	    mov [rsp+8], rax [DONE]
        Label → Memory	        mov [rsp+8], rax (for virtual functions) [DONE]

    
    */


    // DESTINATION IS A REGISTER


    if (auto destReg = std::dynamic_pointer_cast<IRTemp>(moveExp->destination))
    {
        // The source is an immediate value
        if(auto srcImm = std::dynamic_pointer_cast<IRConst>(moveExp->source))
        {
            // Destination: REG, Source: IMMEDIATE

            auto asmInst = std::make_shared<AssemblyMove>(
                Helper::FormatString("mov `d0, %d", srcImm->value),
                destReg->temp,
                nullptr // Source is not a temp
            );

            EmitInstruction(asmInst);
            return;
        }

        // The source is a memory location
        else if (auto srcMem = std::dynamic_pointer_cast<IRMem>(moveExp->source))
        {
            // Destination: REG, SOURCE: MEMORY

            //  memory can be a binop (eg. offset from a register), or a register (the register contains a pointer)

            auto srcBinop = std::dynamic_pointer_cast<IRBinaryOperator>(srcMem->exp);
            if(srcBinop)
            {
                // Only plus and minus are valid
                std::string op = srcBinop->binop == BinaryOperator::PLUS ? "+" : "-";

                // If it's a binop, then it will be an offset from a reg, so the left will be a register, the right a const value

                auto leftTemp = std::dynamic_pointer_cast<IRTemp>(srcBinop->left);
                auto rightConst = std::dynamic_pointer_cast<IRConst>(srcBinop->right);
                assert(leftTemp && rightConst);

                auto asmInst = std::make_shared<AssemblyMove>(
                    Helper::FormatString("mov `d0, [`s0 %s %d]", op.c_str(), rightConst->value),
                    destReg->temp,
                    leftTemp->temp
                );

                EmitInstruction(asmInst);
                return;
            }
            // It's a pointer inside a register
            else if(auto srcReg = std::dynamic_pointer_cast<IRTemp>(srcMem->exp))
            {
                // The memory is at where the register points 

                auto asmInst = std::make_shared<AssemblyMove>(
                    "mov `d0, [`s0]",
                    destReg->temp,
                    srcReg->temp
                );

                EmitInstruction(asmInst);
                return;
            }

            // If non eof them matched, just put source into a temporary and move that into the destination

            auto sourceTemp = MunchExpression(srcMem->exp);

            auto asmInst = std::make_shared<AssemblyMove>(
                "mov `d0, `s0",
                destReg->temp,
                sourceTemp
            );

            EmitInstruction(asmInst);
            return;

        }
        // The source is a label (which is referenced by an IRName obj)
        else if (auto srcName = std::dynamic_pointer_cast<IRName>(srcMem->exp))
        {
            // Destination: REGISTER, Source: LABEL

            auto asmInst = std::make_shared<AssemblyMove>(
                Helper::FormatString("mov `d0, %s", srcName->label->ToString().c_str()),
                destReg->temp,
                nullptr
            );

            EmitInstruction(asmInst);
            return;
        }
    }

    // DESTINATION IS A MEMORY LOCATION

    else if (auto destMem = std::dynamic_pointer_cast<IRMem>(moveExp->destination))
    {
        auto destBinOp = std::dynamic_pointer_cast<IRBinaryOperator>(destMem->exp);

        // The destination is an offset from a register
        if(destBinOp)
        {
            auto destOp = destBinOp->binop == BinaryOperator::PLUS ? "+" : "-";

            auto leftTemp = std::dynamic_pointer_cast<IRTemp>(destBinOp->left);
            auto rightConst = std::dynamic_pointer_cast<IRConst>(destBinOp->right);
            assert(leftTemp && rightConst);

            auto srcImm = std::dynamic_pointer_cast<IRConst>(moveExp->source);
            // Source is an immediate
            if(srcImm)
            {
                // Destination: MEMORY, Source: IMMEDIATE

                auto asmInst = std::make_shared<AssemblyMove>(
                    Helper::FormatString("mov [`d0 %s %d], %d",  destOp, rightConst->value, srcImm->value),
                    leftTemp->temp,
                    nullptr
                );

                EmitInstruction(asmInst);
                return;
            }

            // Source is a register
            else if(auto srcReg = std::dynamic_pointer_cast<IRTemp>(moveExp->source))
            {
                // Destination: MEMORY, Source: REGISTER

                auto asmInst = std::make_shared<AssemblyMove>(
                    Helper::FormatString("mov [`d0 %s %d], `s0",  destOp, rightConst->value),
                    leftTemp->temp,
                    srcReg->temp
                );

                EmitInstruction(asmInst);
                return;

            }
            // Source is a label
            else if (auto srcName = std::dynamic_pointer_cast<IRName>(moveExp->source))
            {
                // Destination: MEMORY, Source: IMMEDIATE

                auto asmInst = std::make_shared<AssemblyMove>(
                    Helper::FormatString("mov [`d0 %s %d], %d",  destOp, rightConst->value, srcName->label->ToString().c_str()),
                    leftTemp->temp,
                    nullptr
                );

                EmitInstruction(asmInst);
                return;
            }

            
            // If non of them matched, just put the source into a temporary and move that to the destination

            auto sourceTemp = MunchExpression(moveExp->source);

            auto asmInst = std::make_shared<AssemblyMove>(
                Helper::FormatString("mov [`d0 %s %d], `s0",destOp, rightConst->value),
                leftTemp->temp,
                sourceTemp
            );

            EmitInstruction(asmInst);
            return;
        }
    }
    
}
void x86CodeGenerator::MunchPop(std::shared_ptr<IRPop> popExp)
{
    // Pop can only be used with Temp or MEM


    if (auto irTemp = std::dynamic_pointer_cast<IRTemp>(popExp->exp))
    {
        auto destTemp = irTemp->temp;

        auto asmInst = std::make_shared<AssemblyOper>(
            "pop `d0",
            AppendTempList(destTemp,nullptr),
            nullptr
        );

        EmitInstruction(asmInst);
    }

    else if (auto irMem = std::dynamic_pointer_cast<IRMem>(popExp->exp)) {
        auto addrTemp = MunchExpression(irMem->exp);

        auto asmInst = std::make_shared<AssemblyOper>(
            "pop [`d0]",  
            AppendTempList(addrTemp, nullptr),
            nullptr
        );

        EmitInstruction(asmInst);
    }
    else {
        throw std::runtime_error("Invalid operand for pop!");
    } 
}
void x86CodeGenerator::MunchPush(std::shared_ptr<IRPush> pushExp)
{
    auto source = MunchExpression(pushExp->exp);

    auto asmInst = std::make_shared<AssemblyOper>(
        "push `s0", // Push the first source
        nullptr, // There are no destination values
        AppendTempList(source,nullptr) // The source is the inner expression
    );

    EmitInstruction(asmInst);

}
void x86CodeGenerator::MunchReturn(std::shared_ptr<IRReturn> exp)
{
    auto asmInst = std::make_shared<AssemblyOper>(
        "ret",
        nullptr,
        nullptr
    );

    EmitInstruction(asmInst);
}
void x86CodeGenerator::MunchSysCall(std::shared_ptr<IRSysCall> exp)
{
    auto asmInst = std::make_shared<AssemblyOper>(
        "syscall",
        nullptr,
        nullptr
    );

    EmitInstruction(asmInst);
}

std::shared_ptr<Temp> x86CodeGenerator::MunchBinaryOperator(std::shared_ptr<IRBinaryOperator> binaryOpExp)
{
    // ADDITION

    if(binaryOpExp->binop == BinaryOperator::PLUS)
    {
        // The temporaries that hold the value of the two operands

        auto leftTemp = MunchExpression(binaryOpExp->left);
        auto rightTemp = MunchExpression(binaryOpExp->right);

        // The destination will be the left temporary
        auto destList = AppendTempList(leftTemp,nullptr);

        // The source list will contain both temporaries
        auto srcList = AppendTempList(leftTemp, AppendTempList(rightTemp,nullptr));

        auto asmInst = std::make_shared<AssemblyOper>(
            "add `d0, `s1",
            destList,
            srcList
        );

        EmitInstruction(asmInst);

        return leftTemp;

    }

    // SUBTRACTION (similiar to addition)

    else if (binaryOpExp->binop == BinaryOperator::MINUS)
    {
        // The temporaries that hold the value of the two operands

        auto leftTemp = MunchExpression(binaryOpExp->left);
        auto rightTemp = MunchExpression(binaryOpExp->right);

        // The destination will be the left temporary
        auto destList = AppendTempList(leftTemp,nullptr);

        // The source list will contain both temporaries
        auto srcList = AppendTempList(leftTemp, AppendTempList(rightTemp,nullptr));

        auto asmInst = std::make_shared<AssemblyOper>(
            "sub `d0, `s1",
            destList,
            srcList
        );

        EmitInstruction(asmInst);

        return leftTemp;
    }

    // MULTIPLICATION

    else if (binaryOpExp->binop == BinaryOperator::MULTIPLY)
    {

        auto leftTemp = MunchExpression(binaryOpExp->left);
        auto rightTemp = MunchExpression(binaryOpExp->right);

        auto asmInst = std::make_shared<AssemblyOper>(
            "imul `d0, `s1",
            AppendTempList(leftTemp,nullptr),
            AppendTempList(leftTemp,AppendTempList(rightTemp,nullptr)) // Both registers count as sources
        );

        EmitInstruction(asmInst);

        return leftTemp;
    }


    // INTEGER DIVISION

    else if (binaryOpExp->binop == BinaryOperator::DIVIDE)
    {
        // Need to set rdx to 0 before division

        auto nullRDX = std::make_shared<AssemblyOper>(
            "xor `d0,`s0",
            AppendTempList(ReservedIrRegisters::RDX, nullptr),
            AppendTempList(ReservedIrRegisters::RDX,nullptr)
        );

        EmitInstruction(nullRDX);

        auto leftTemp = MunchExpression(binaryOpExp->left);
        auto rightTemp = MunchExpression(binaryOpExp->right);

        // Move left operand into rax

        auto moveIntoRax = std::make_shared<AssemblyMove>(
            "mov `d0, `s0",
            ReservedIrRegisters::RAX,
            leftTemp
        );

        EmitInstruction(moveIntoRax);

        // Sign extend RAX into RDX:RAX

        auto signExtend = std::make_shared<AssemblyOper>(
            "cqto",
            AppendTempList(ReservedIrRegisters::RDX,nullptr),
            AppendTempList(ReservedIrRegisters::RAX,nullptr)
        );

        EmitInstruction(signExtend);

        // Divide the right operand with RAX
        auto division = std::make_shared<AssemblyOper>(
            "idiv `s0",
            AppendTempList(ReservedIrRegisters::RAX,nullptr),
            AppendTempList(rightTemp,nullptr)
        );

        EmitInstruction(division);

        // The result will be in RAX
        return ReservedIrRegisters::RAX;


    }

    throw std::logic_error("Invalid BinaryOperator operator!");

}

std::shared_ptr<Temp> x86CodeGenerator::MunchFunctionCall(std::shared_ptr<IRCall> exp)
{
    // Function calls can only appear inside Evaluate and Move nodes!

    throw std::logic_error("Standalone function call encountered in code generation!");
}

std::shared_ptr<Temp> x86CodeGenerator::MunchConstFloat(std::shared_ptr<IRConstFloat> floatExp)
{
    //TODO: Make sure a float reg is assigned here
    auto destTemp = std::make_shared<Temp>();

    auto asmInst = std::make_shared<AssemblyMove>(
        Helper::FormatString("movss `d0, %f", floatExp->value),
        destTemp,
        nullptr // There is no source here
    );

    EmitInstruction(asmInst);

    return destTemp;
}

std::shared_ptr<Temp> x86CodeGenerator::MunchConstInteger(std::shared_ptr<IRConst> constExp)
{
    auto destinationTemp = std::make_shared<Temp>();

    auto asmInst = std::make_shared<AssemblyMove>(
        Helper::FormatString("mov `d0, %d", constExp->value),
        destinationTemp,
        nullptr // There is no source
    );

    EmitInstruction(asmInst);

    return destinationTemp;
}
std::shared_ptr<Temp> x86CodeGenerator::MunchMem(std::shared_ptr<IRMem> memExp)
{
    // Need to move the inner expression into a reg and that move that value into the newLocation of the Mem expression
    auto addressTemp = MunchExpression(memExp->exp);

    auto newLocation = std::make_shared<Temp>();

    auto asmInst = std::make_shared<AssemblyMove>(
        "mov `d0, [`s0]", // Move the first source into the first destination
        newLocation,
        addressTemp
    );

    EmitInstruction(asmInst);

    return newLocation;
}

std::shared_ptr<Temp> x86CodeGenerator::MunchName(std::shared_ptr<IRName> nameExp)
{
    // FIXME: May need to implement MunchName in a way that the label name is not hardcoded into the asm string

    auto destTemp = std::make_shared<Temp>();

    auto labelName = nameExp->label->ToString();

    auto asmInst = std::make_shared<AssemblyMove>(
        Helper::FormatString("mov `d0, %s",labelName.c_str()), // Move the label into the reg
        destTemp,
        nullptr
    );

    EmitInstruction(asmInst);

    return destTemp;
}

std::shared_ptr<Temp> x86CodeGenerator::MunchTemp(std::shared_ptr<IRTemp> exp)
{
    // RSP and RBP cannot be used directly ,so whenever they are referenced in an expression (move will deal with them on its own) they need to be copied and the other one referenced
    auto givenTemp = exp->temp;

    if(givenTemp == ReservedIrRegisters::StackPointer || givenTemp == ReservedIrRegisters::FramePointer)
    {
        auto newTemp = std::make_shared<Temp>();

        auto moveSpecial = std::make_shared<AssemblyMove>(
            "mov `d0, `s0",
            newTemp,
            givenTemp
        );

        EmitInstruction(moveSpecial);

        return newTemp;
    }

    // If not special, just return as-is
    return exp->temp;
}
