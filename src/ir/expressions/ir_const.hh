#pragma once
#include "ir_expression.hh"

// Integer constant
class IRConst : IRExpression
{
        int value;
    public:
        IRConst(int);
};