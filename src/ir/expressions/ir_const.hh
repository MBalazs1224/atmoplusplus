#pragma once
#include "ir_expression.hh"

// Integer constant
class IRConst : public IRExpression
{
        int value;
    public:
        IRConst(int);

        std::string ToDotFormat(int&) override;
};