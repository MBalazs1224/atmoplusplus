#pragma once

#include "ir_expression.hh"

// Integer constant
class IRConstFloat : public IRExpression
{
        int value;
    public:
        IRConstFloat(int);
};