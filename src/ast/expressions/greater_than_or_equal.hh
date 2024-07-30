#pragma once
#include "expression.hh"
class GreaterThanOrEqualExpression : public Expression
{

    public:
    GreaterThanOrEqualExpression(std::unique_ptr<IExpressionable> left_in, std::unique_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~GreaterThanOrEqualExpression() override = default;
    std::string Type() override
    {
        return "GreaterThanOrEqual";
    }
};