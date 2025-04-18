#include "inreg.hh"
#include "../ir/expressions/ir_temp.hh"
InReg::InReg(DataSize space)
{
    temp = std::make_shared<Temp>(space);
}

InReg::InReg(std::shared_ptr<Temp> incomingTemp)
{
    this->temp = incomingTemp;
}

std::string InReg::ToString()
{
    return temp->ToString();
}

std::shared_ptr<IRExpression> InReg::AsExpression(std::shared_ptr<IRExpression> framePointer)
{
    // If the location is a register then we don't have to do any calculations, just use the register

    return std::make_shared<IRTemp>(temp);
}