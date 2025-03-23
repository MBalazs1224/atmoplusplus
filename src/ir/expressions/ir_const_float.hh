#pragma once

#include "ir_expression.hh"

// Integer constant
class IRConstFloat : public IRExpression
{
    public:
        double value;
        IRConstFloat(double);

        std::string ToDotFormat(int&) override;
};