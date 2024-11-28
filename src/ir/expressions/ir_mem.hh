#pragma once
#include <memory>
#include "ir_expression.hh"


// Represents wordSize bytes of memory starting at exp location
class IRMem : IRExpression
{
        std::unique_ptr<IRExpression> exp;
    public:
        IRMem(std::unique_ptr<IRExpression>);
};