#pragma once

#include "ir_expression.hh"

// Integer constant
class IRConstFloat : public IRExpression
{
        double value;
    public:
        IRConstFloat(double);

        std::string ToDotFormat(int&) override;
};