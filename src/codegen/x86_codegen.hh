#pragma once
#include "codegen.hh"

class x86CodeGenerator : public CodeGenerator
{
    virtual void MunchStatement(std::shared_ptr<IRStatement> stmt) override;
    virtual std::shared_ptr<IRTemp> MunchExpression(std::shared_ptr<IRExpression> exp) override;
};