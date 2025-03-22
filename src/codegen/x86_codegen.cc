#include "x86_codegen.hh"


void x86CodeGenerator::MunchLabel(std::shared_ptr<IRLabel> exp) {}


void x86CodeGenerator::MunchCjump(std::shared_ptr<IRCJump> exp) {}

void x86CodeGenerator::MunchEnter(std::shared_ptr<IREnter> exp) {}

void x86CodeGenerator::MunchEvaluateExpression(std::shared_ptr<IREvaluateExpression> exp) {}

void x86CodeGenerator::MunchJump(std::shared_ptr<IRJump> exp) {}

void x86CodeGenerator::MunchLeave(std::shared_ptr<IRLeave> exp) {}

void x86CodeGenerator::MunchMove(std::shared_ptr<IRMove> exp) {}
void x86CodeGenerator::MunchPop(std::shared_ptr<IRPop> exp) {}
void x86CodeGenerator::MunchPush(std::shared_ptr<IRPush> exp) {}
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

std::shared_ptr<Temp> x86CodeGenerator::MunchMem(std::shared_ptr<IRMem> exp)
{
    return nullptr;
}

std::shared_ptr<Temp> x86CodeGenerator::MunchName(std::shared_ptr<IRName> exp)
{
    return nullptr;
}

std::shared_ptr<Temp> x86CodeGenerator::MunchTemp(std::shared_ptr<IRTemp> exp)
{
    return nullptr;
}
