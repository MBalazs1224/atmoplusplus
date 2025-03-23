#pragma once
#include "ir_expression.hh"

// Integer constant
class IRConst : public IRExpression
{
    public:
        int value;
        IRConst(int);

        std::string ToDotFormat(int&) override;
};