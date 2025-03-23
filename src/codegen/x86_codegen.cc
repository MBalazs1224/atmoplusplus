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


void x86CodeGenerator::MunchCjump(std::shared_ptr<IRCJump> exp) {}

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

void x86CodeGenerator::MunchMove(std::shared_ptr<IRMove> exp) {}
void x86CodeGenerator::MunchPop(std::shared_ptr<IRPop> exp) {}
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

std::shared_ptr<Temp> x86CodeGenerator::MunchBinaryOperator(std::shared_ptr<IRBinaryOperator> exp)
{
    return nullptr;
}

std::shared_ptr<Temp> x86CodeGenerator::MunchFunctionCall(std::shared_ptr<IRCall> exp)
{
    return nullptr;
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
