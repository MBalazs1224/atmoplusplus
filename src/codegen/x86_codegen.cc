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

void x86CodeGenerator::MunchEnter(std::shared_ptr<IREnter> exp) {}

void x86CodeGenerator::MunchEvaluateExpression(std::shared_ptr<IREvaluateExpression> exp) {}

void x86CodeGenerator::MunchJump(std::shared_ptr<IRJump> exp) {}

void x86CodeGenerator::MunchLeave(std::shared_ptr<IRLeave> exp) {}

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
void x86CodeGenerator::MunchReturn(std::shared_ptr<IRReturn> exp) {}
void x86CodeGenerator::MunchSysCall(std::shared_ptr<IRSysCall> exp) {}

std::shared_ptr<Temp> x86CodeGenerator::MunchBinaryOperator(std::shared_ptr<IRBinaryOperator> exp)
{
    return nullptr;
}

std::shared_ptr<Temp> x86CodeGenerator::MunchFunctionCall(std::shared_ptr<IRCall> exp)
{
    return nullptr;
}

std::shared_ptr<Temp> x86CodeGenerator::MunchConstFloat(std::shared_ptr<IRConstFloat> exp)
{
    return nullptr;
}

std::shared_ptr<Temp> x86CodeGenerator::MunchConstInteger(std::shared_ptr<IRConst> exp)
{
    return nullptr;
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

std::shared_ptr<Temp> x86CodeGenerator::MunchName(std::shared_ptr<IRName> exp)
{
    return nullptr;
}

std::shared_ptr<Temp> x86CodeGenerator::MunchTemp(std::shared_ptr<IRTemp> exp)
{
    return exp->temp;
}
