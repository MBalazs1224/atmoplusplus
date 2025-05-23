#include "x86_codegen.hh"


void x86CodeGenerator::MunchLabel(std::shared_ptr<IRLabel> exp)
{
    auto innerLabel = exp->label;
    auto labelName = innerLabel->ToString();
    auto labelAsm = std::make_shared<AssemblyLabel>(
        Helper::FormatString("%s:", labelName.c_str()), // In asm the label is followed by a ':'
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

    auto newLeftTemp = leftTemp->Clone(rightTemp->sizeNeeded);

    auto cmpIns = std::make_shared<AssemblyOper>(
        "cmp `s0, `s1",
        nullptr, // No destination
        AppendTempList(newLeftTemp, AppendTempList(rightTemp,nullptr))
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

        // MOVE THE ARGUMENTS TO THE CORRECT LOCATION


        auto argument = call->args;
        auto paramLocation = call->argumentLocations;

        while (argument)
        {
            // Argument and param locations must match at this point

            auto srcTemp = MunchExpression(argument->expression);
            auto destTemp = MunchExpression(paramLocation->expression);


            auto newDestTemp = destTemp->Clone(srcTemp->sizeNeeded);

            auto newSrcTemp = srcTemp->Clone(newDestTemp->sizeNeeded);

            auto asmInst =std::make_shared<AssemblyMove>(
                "mov `d0, `s0",
                newDestTemp,
                newSrcTemp
            );
            EmitInstruction(asmInst);

            argument = argument->next;
            paramLocation = paramLocation->next;
        }

        // The value of caller saved registers + return register must be given as destinations, so later parts of the compiler knows that something happens to them here
        auto callDefs = GlobalFrame::globalFrameType->GetCallDefs();

        
        

        if(auto name = std::dynamic_pointer_cast<IRName>(call->func))
        {
            // If a label is called, just call the label without an intermediate temp

            auto asmIns = std::make_shared<AssemblyOper>(
                "call `j0",
                callDefs,
                nullptr,
                std::make_shared<LabelList>(name->label, nullptr)
            );
            EmitInstruction(asmIns);
            return;
        }
        else
        {
            // kNow where the func is
            auto newTemp = MunchExpression(call->func);

            auto finalTemp = newTemp->Clone(DataSize::QWord); // 64 bit address
            
            auto asmInst = std::make_shared<AssemblyOper>(
                "call `s0",
                callDefs,
                AppendTempList(finalTemp, nullptr)
            );
            EmitInstruction(asmInst);
        
        }
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

std::string x86CodeGenerator::SizeToString(int size)
{
    switch (size)
    {
    case 1:
        return "byte";
    
    case 2:
        return "word";

    case 4:
        return "dword";
    
    case 8:
        return ""; // 8 byte size is the default, so no additional size instruction is needed

    default:
        throw std::logic_error("Invalid size in SizeToString!");
    }
}

std::shared_ptr<Temp> x86CodeGenerator::MunchLoadEffectiveAddress(std::shared_ptr<IRLoadEffectiveAddress> lea)
{
    // It can only load memory address to register

    auto destTemp = std::make_shared<Temp>(DataSize::QWord); // Holds 64 bit address
    
    if(auto srcMem = std::dynamic_pointer_cast<IRMem>(lea->source))
        {
            if (auto srcBinop = std::dynamic_pointer_cast<IRBinaryOperator>(srcMem->exp)) 
            {
                auto leftTemp = std::dynamic_pointer_cast<IRTemp>(srcBinop->left);
                auto rightConst = std::dynamic_pointer_cast<IRConst>(srcBinop->right);
                if (leftTemp && rightConst) 
                {
                    auto newSource = leftTemp->temp->Clone(DataSize::QWord); // Holds a 64 bit address

                    const char* op = srcBinop->binop == BinaryOperator::PLUS ? "+" : "-";

                    EmitInstruction(std::make_shared<AssemblyMove>(
                        Helper::FormatString("lea `d0, [`s0 %s %d]", 
                            op, 
                            rightConst->value),
                        destTemp,
                        newSource
                    ));
                    return destTemp;
                }
            }
        }
    
    throw std::logic_error("Invalid expressions inside MunchLoadEffectiveAddress!");
}

void x86CodeGenerator::MunchMove(std::shared_ptr<IRMove> moveExp) 
{
    // If the source is a function call, we need to first move the return value to the correct place
    if (auto call = std::dynamic_pointer_cast<IRCall>(moveExp->source))
    {

        // MOVE THE ARGUMENTS TO THE CORRECT LOCATION


        auto argument = call->args;
        auto paramLocation = call->argumentLocations;

        while (argument)
        {
            // Argument and param locations must match at this point

            auto srcTemp = MunchExpression(argument->expression);
            auto destTemp = MunchExpression(paramLocation->expression);


            auto newDestTemp = destTemp->Clone(srcTemp->sizeNeeded);
            auto newSrcTemp = srcTemp->Clone(newDestTemp->sizeNeeded);

            auto asmInst =std::make_shared<AssemblyMove>(
                "mov `d0, `s0",
                newDestTemp,
                newSrcTemp
            );
            EmitInstruction(asmInst);

            argument = argument->next;
            paramLocation = paramLocation->next;
        }


        // CALL THE FUNCTION

        auto callDefs = GlobalFrame::globalFrameType->GetCallDefs();

        // If it's a label then we just call the function regularly
        if(auto name = std::dynamic_pointer_cast<IRName>(call->func))
        {
            auto asmInst = std::make_shared<AssemblyOper>(
                "call `j0",
                callDefs,
                nullptr,
                std::make_shared<LabelList>(
                    name->label,
                    nullptr
                )
            );

            EmitInstruction(asmInst);
        }
        else
        {
            auto funcLocation = MunchExpression(call->func);

            auto newLocation = funcLocation->Clone(DataSize::QWord); // 64 bit adddress

            auto asmInst = std::make_shared<AssemblyOper>(
                "call `s0",
                callDefs,
                std::make_shared<TempList>(
                    newLocation,
                    nullptr
                ),
                nullptr
            );

            EmitInstruction(asmInst);
        }

        // MOVE RETURN REGISTER TO CORRECT LOCATION

        auto returnTemp = ReservedIrRegisters::RAX; // FIXME: Needs to be mroe flexible for different architectures

        auto destinationTemp = MunchExpression(moveExp->destination);


        auto newReturn = returnTemp->Clone(call->returnSize);

        auto newDestTemp = destinationTemp->Clone(newReturn->sizeNeeded);

        auto asmInst = std::make_shared<AssemblyMove>(
            "mov `d0, `s0",
            newDestTemp,
            newReturn
        );

        EmitInstruction(asmInst);

        return;

    }


    // Register destination
    if (auto destReg = std::dynamic_pointer_cast<IRTemp>(moveExp->destination)) 
    {
        // Immediate to register
        if (auto srcImm = std::dynamic_pointer_cast<IRConst>(moveExp->source)) 
        {

            auto destTemp = destReg->temp->Clone(DataSize::DWord);

            EmitInstruction(std::make_shared<AssemblyMove>(
                Helper::FormatString("mov `d0, %d", srcImm->value),
                destReg->temp,
                destTemp
            ));
            return;
        }

        // Memory to register
        if (auto srcMem = std::dynamic_pointer_cast<IRMem>(moveExp->source)) 
        {
            auto newDestReg = destReg->temp->Clone((DataSize)srcMem->bytesNeeded);
            
            if (auto srcBinop = std::dynamic_pointer_cast<IRBinaryOperator>(srcMem->exp)) 
            {
                auto leftTemp = std::dynamic_pointer_cast<IRTemp>(srcBinop->left);
                auto rightConst = std::dynamic_pointer_cast<IRConst>(srcBinop->right);
                if (leftTemp && rightConst) 
                {
                    auto newSource = leftTemp->temp->Clone(DataSize::QWord); // Holds a 64 bit address

                    const char* op = srcBinop->binop == BinaryOperator::PLUS ? "+" : "-";
                    EmitInstruction(std::make_shared<AssemblyMove>(
                        Helper::FormatString("mov `d0, %s [`s0 %s %d]", 
                            SizeToString(srcMem->bytesNeeded).c_str(), 
                            op, 
                            rightConst->value),
                        newDestReg,
                        newSource
                    ));
                    return;
                }
            }

            if (auto srcReg = std::dynamic_pointer_cast<IRTemp>(srcMem->exp)) 
            {
                auto newSource = srcReg->temp->Clone(DataSize::QWord);

                EmitInstruction(std::make_shared<AssemblyMove>(
                    "mov `d0, [`s0]",
                    destReg->temp,
                    newSource
                ));
                return;
            }
        }

        // Label to register
        if (auto srcName = std::dynamic_pointer_cast<IRName>(moveExp->source)) 
        {

            auto destTemp = destReg->temp->Clone(DataSize::QWord);
            EmitInstruction(std::make_shared<AssemblyMove>(
                Helper::FormatString("mov `d0, %s", srcName->label->ToString().c_str()),
                destTemp,
                nullptr
            ));
            return;
        }

        // General
        auto sourceTemp = MunchExpression(moveExp->source);

        auto destTemp = destReg->temp->Clone(sourceTemp->sizeNeeded);

        EmitInstruction(std::make_shared<AssemblyMove>(
            "mov `d0, `s0",
            destTemp,
            sourceTemp
        ));
        return;
    }

    // Memory destinations
    if (auto destMem = std::dynamic_pointer_cast<IRMem>(moveExp->destination)) 
    {
        // mov [REG + CONST], source
        if(auto binOp = std::dynamic_pointer_cast<IRBinaryOperator>(destMem->exp))
            if(auto rightConst = std::dynamic_pointer_cast<IRConst>(binOp->right))
                if(auto leftTemp = std::dynamic_pointer_cast<IRTemp>(binOp->left))
                {
                    // It could only be plus or minus
                    auto op = binOp->binop == BinaryOperator::PLUS ? "+" : "-";

                    // source is immediate
                    if(auto srcImm = std::dynamic_pointer_cast<IRConst>(moveExp->source))
                    {
                        auto newLeftTemp = leftTemp->temp->Clone(DataSize::QWord); // Has to be a 64 bit register for a memory address

                        auto asmInst = std::make_shared<AssemblyMove>(
                            Helper::FormatString("mov dword [`d0 %s %d], %d", op, rightConst->value, srcImm->value),
                            newLeftTemp,
                            nullptr
                        );

                        EmitInstruction(asmInst);
                        return;
                    }

                    auto source = MunchExpression(moveExp->source);

                    auto newSource = source->Clone((DataSize)destMem->bytesNeeded);

                    auto sizeString = SizeToString(source->sizeNeeded);

                    auto newDest = leftTemp->temp->Clone(DataSize::QWord); // Holds a 64 bit address
                    
                    auto asmInst = std::make_shared<AssemblyMove>(
                        Helper::FormatString("mov %s [`d0 %s %d], `s0",sizeString.c_str(), op, rightConst->value),
                        newDest,
                        newSource
                    );

                    EmitInstruction(asmInst);
                    return;
                }

        const auto destSize = SizeToString(destMem->bytesNeeded);

        // Memory to memory through a temporary register
        if (auto srcMem = std::dynamic_pointer_cast<IRMem>(moveExp->source)) 
        {
            auto temp = MunchExpression(srcMem);
            auto addr = MunchExpression(destMem->exp);

            auto newAddr = addr->Clone(DataSize::QWord); // Holds a 64 bit value

            auto newTemp = temp->Clone((DataSize)srcMem->bytesNeeded);


            EmitInstruction(std::make_shared<AssemblyMove>(
                Helper::FormatString("mov %s [`d0], `s0", destSize.c_str()),
                newAddr,
                newTemp
            ));
            return;
        }

        // Complex addressing modes
        if (auto destBinOp = std::dynamic_pointer_cast<IRBinaryOperator>(destMem->exp)) 
        {
            auto addr = MunchExpression(destBinOp);
            auto src = MunchExpression(moveExp->source);

            auto newSrc = src->Clone(addr->sizeNeeded);

            auto newAddr = addr->Clone(DataSize::QWord); // Holds a 64 bit address

            EmitInstruction(std::make_shared<AssemblyMove>(
                Helper::FormatString("mov %s [`d0], `s0", destSize.c_str()),
                newAddr,
                newSrc
            ));
            return;
        }

        // Immediate to memory
        if (auto srcImm = std::dynamic_pointer_cast<IRConst>(moveExp->source)) 
        {
            auto addr = MunchExpression(destMem->exp);
            EmitInstruction(std::make_shared<AssemblyMove>(
                Helper::FormatString("mov %s [`d0], %d", destSize.c_str(), srcImm->value),
                addr,
                nullptr
            ));
            return;
        }

        // Register to memory
        if (auto srcReg = std::dynamic_pointer_cast<IRTemp>(moveExp->source)) 
        {
            auto addr = MunchExpression(destMem->exp);

            auto newAddr = addr->Clone(DataSize::QWord); // Holds a 64 bit address

            auto newSrc = srcReg->temp->Clone((DataSize)destMem->bytesNeeded);

            EmitInstruction(std::make_shared<AssemblyMove>(
                Helper::FormatString("mov %s [`d0], `s0", destSize.c_str()),
                addr,
                newSrc
            ));
            return;
        }

        // General fallback for memory

        auto address = MunchExpression(destMem->exp);

        auto newAddress = address->Clone(DataSize::QWord); // Holds 64 bit address

        auto source = MunchExpression(moveExp->source);

        auto sizeString = SizeToString(source->sizeNeeded);

        auto asmInst = std::make_shared<AssemblyMove>(
            Helper::FormatString("mov %s [`d0], `s0", sizeString.c_str()),
            newAddress,
            source
        );

        EmitInstruction(asmInst);
        return;
    }

    // General fallback
    auto source = MunchExpression(moveExp->source);
    auto dest = MunchExpression(moveExp->destination);
    
    auto newDest = dest->Clone(source->sizeNeeded);

    EmitInstruction(std::make_shared<AssemblyMove>(
        "mov `d0, `s0",
        newDest,
        source
    ));
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

        auto sizeString = SizeToString(irMem->bytesNeeded);

        auto asmInst = std::make_shared<AssemblyOper>(
            Helper::FormatString("pop %s [`d0]", sizeString.c_str()),  
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
        auto leftTemp = MunchExpression(binaryOpExp->left);

        // The destination will be the left temporary


        // If the source is a const we can just use that without an intermediate temp
        if(auto srcImm = std::dynamic_pointer_cast<IRConst>(binaryOpExp->right))
        {
            auto newLeftTemp = leftTemp->Clone(DataSize::DWord);

            auto newDestList = AppendTempList(newLeftTemp,nullptr);

            auto asmIns = std::make_shared<AssemblyOper>(
                Helper::FormatString("add `d0, %d", srcImm->value),
                newDestList,
                nullptr
            );

            EmitInstruction(asmIns);

            return newLeftTemp;
        }
        // The temporaries that hold the value of the two operands

        auto rightTemp = MunchExpression(binaryOpExp->right);


        auto newLeftTemp = leftTemp->Clone(rightTemp->sizeNeeded);
        auto newDestList = AppendTempList(newLeftTemp, nullptr);

        // The source list will contain both temporaries
        auto srcList = AppendTempList(newLeftTemp, AppendTempList(rightTemp,nullptr));
        auto asmInst = std::make_shared<AssemblyOper>(
            "add `d0, `s1",
            newDestList,
            srcList
        );

        EmitInstruction(asmInst);

        return newLeftTemp;

    }

    // SUBTRACTION (similiar to addition)

    else if (binaryOpExp->binop == BinaryOperator::MINUS)
    {
        auto leftTemp = MunchExpression(binaryOpExp->left);
        auto destList = AppendTempList(leftTemp,nullptr);

        // If the source is a const we can just use that without an intermediate temp
        if(auto srcImm = std::dynamic_pointer_cast<IRConst>(binaryOpExp->right))
        {
            auto asmIns = std::make_shared<AssemblyOper>(
                Helper::FormatString("sub `d0, %d", srcImm->value),
                destList,
                nullptr
            );

            EmitInstruction(asmIns);

            return leftTemp;
        }
        // The temporaries that hold the value of the two operands

        auto rightTemp = MunchExpression(binaryOpExp->right);

        // The destination will be the left temporary

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

    // MULTIPLICATION FIXME: Maybe the result register EDX I think where the upper half is put, should be in the destination list

    else if (binaryOpExp->binop == BinaryOperator::MULTIPLY)
    {
        auto leftTemp = MunchExpression(binaryOpExp->left);

        // If the source is a const we can just use that without an intermediate temp
        if(auto srcImm = std::dynamic_pointer_cast<IRConst>(binaryOpExp->right))
        {
            auto asmIns = std::make_shared<AssemblyOper>(
                Helper::FormatString("imul `d0, %d", srcImm->value),
                AppendTempList(leftTemp,nullptr),
                AppendTempList(leftTemp,nullptr)
            );

            EmitInstruction(asmIns);

            return leftTemp;
        }

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
    auto destTemp = std::make_shared<Temp>(DataSize::DWord); // Floats are 4 bytes

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
    auto destinationTemp = std::make_shared<Temp>(DataSize::DWord); // Integers are 4 bytes

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
    

    auto newLocation = std::make_shared<Temp>((DataSize)memExp->bytesNeeded);

    auto sizeString = SizeToString(memExp->bytesNeeded);

     // mov reg , [reg + const]
    if(auto binOp = std::dynamic_pointer_cast<IRBinaryOperator>(memExp->exp))
        if(auto leftReg = std::dynamic_pointer_cast<IRTemp>(binOp->left))
            if(auto rightConst = std::dynamic_pointer_cast<IRConst>(binOp->right))
            {
                auto binop = binOp->binop == BinaryOperator::PLUS ? "+" : "-";
                auto asmInst = std::make_shared<AssemblyMove>(
                    Helper::FormatString("mov `d0, %s [`s0 %s %d]", sizeString.c_str(), binop, rightConst->value),
                    newLocation,
                    leftReg->temp
                );

                EmitInstruction(asmInst);

                return newLocation;
            }

    // Need to move the inner expression into a reg and that move that value into the newLocation of the Mem expression
    auto addressTemp = MunchExpression(memExp->exp);

    auto newAddr = addressTemp->Clone(DataSize::QWord); // Holds a 64 bit address

    auto asmInst = std::make_shared<AssemblyMove>(
        Helper::FormatString("mov `d0, %s [`s0]", sizeString.c_str()), // Move the first source into the first destination
        newLocation,
        newAddr
    );

    EmitInstruction(asmInst);

    return newLocation;
}

std::shared_ptr<Temp> x86CodeGenerator::MunchName(std::shared_ptr<IRName> nameExp)
{
    // FIXME: May need to implement MunchName in a way that the label name is not hardcoded into the asm string

    auto destTemp = std::make_shared<Temp>(DataSize::QWord); //  Will be a pointer 

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
        auto newTemp = std::make_shared<Temp>(givenTemp->sizeNeeded);

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
