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
        "cmp s0, s1",
        nullptr, // No destination
        AppendTempList(leftTemp, AppendTempList(rightTemp,nullptr))
    );

    EmitInstruction(cmpIns);

    std::string jumpOp = RelationalOperatorToString(cJumpExp->relop);

    auto jumpTrue = std::make_shared<AssemblyOper>(
        Helper::FormatString("%s j0", jumpOp.c_str()),
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
            "call s0",
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
        "jmp j0",
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
    // The destination can either be a memory location or a register
    if(auto regDest = std::dynamic_pointer_cast<IRTemp>(moveExp->destination))
    {
        auto destTemp = regDest->temp;
        auto sourceTemp = MunchExpression(moveExp->source);

        auto asmInst = std::make_shared<AssemblyMove>(
            "mov d0, s0",
            destTemp,
            sourceTemp
        );

        EmitInstruction(asmInst);
    }
    else if(auto memDest = std::dynamic_pointer_cast<IRMem>(moveExp->destination))
    {
        auto memoryLocation = MunchExpression(memDest->exp);
        // Further optimization to move const directly without intermadiate register
        if(auto constValue = std::dynamic_pointer_cast<IRConst>(moveExp->source))
        {
            auto asmInst = std::make_shared<AssemblyMove>(
                Helper::FormatString("mov qword ptr [d0], %d", constValue->value),
                memoryLocation,
                nullptr // No source
            );
            EmitInstruction(asmInst);

        }

        else
        {
            auto sourceTemp = MunchExpression(moveExp->source);

            auto asmInst = std::make_shared<AssemblyMove>(
                "mov qword ptr [d0], s0",
                memoryLocation,
                sourceTemp
            );

            EmitInstruction(asmInst);
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
            "pop d0",
            AppendTempList(destTemp,nullptr),
            nullptr
        );

        EmitInstruction(asmInst);
    }

    else if (auto irMem = std::dynamic_pointer_cast<IRMem>(popExp->exp)) {
        auto addrTemp = MunchExpression(irMem->exp);

        auto asmInst = std::make_shared<AssemblyOper>(
            "pop [d0]",  
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
        "push s0", // Push the first source
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
            "add d0, s1",
            destList,
            srcList
        );

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
            "sub d0, s1",
            destList,
            srcList
        );

        return leftTemp;
    }

    // MULTIPLICATION

    else if (binaryOpExp->binop == BinaryOperator::MULTIPLY)
    {

        auto leftTemp = MunchExpression(binaryOpExp->left);
        auto rightTemp = MunchExpression(binaryOpExp->right);

        auto asmInst = std::make_shared<AssemblyOper>(
            "imul d0, s1",
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
            "xor d0,s0",
            AppendTempList(ReservedIrRegisters::RDX, nullptr),
            AppendTempList(ReservedIrRegisters::RDX,nullptr)
        );

        EmitInstruction(nullRDX);

        auto leftTemp = MunchExpression(binaryOpExp->left);
        auto rightTemp = MunchExpression(binaryOpExp->right);

        // Move left operand into rax

        auto moveIntoRax = std::make_shared<AssemblyMove>(
            "mov d0, s0",
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
            "idiv s0",
            AppendTempList(ReservedIrRegisters::RAX,nullptr),
            AppendTempList(rightTemp,nullptr)
        );

        EmitInstruction(division);

        // The result will be in RAX
        return ReservedIrRegisters::RAX;


    }

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
        Helper::FormatString("movss d0, %f", floatExp->value),
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
        Helper::FormatString("mov d0, %d", constExp->value),
        destinationTemp,
        nullptr // There is no source
    );

    EmitInstruction(asmInst);

    return destinationTemp;
}
std::shared_ptr<Temp> x86CodeGenerator::MunchMem(std::shared_ptr<IRMem> memExp)
{
    // Need to move the inner expression into a reg and that move that value into the newLocation of the Mem expression
    auto addressTemp = MunchExpression(memExp);

    auto newLocation = std::make_shared<Temp>();

    auto asmInst = std::make_shared<AssemblyMove>(
        "mov d0, [s0]", // Move the first source into the first destination
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
        Helper::FormatString("mov d0, %s",labelName.c_str()), // Move the label into the reg
        destTemp,
        nullptr
    );

    return destTemp;
}

std::shared_ptr<Temp> x86CodeGenerator::MunchTemp(std::shared_ptr<IRTemp> exp)
{
    return exp->temp;
}
