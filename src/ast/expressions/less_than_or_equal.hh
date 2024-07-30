#pragma once
#include "expression.hh"
class LessThanOrEqualExpression : public Expression
{
    public:
    LessThanOrEqualExpression(std::unique_ptr<IExpressionable> left_in, std::unique_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~LessThanOrEqualExpression() override = default;
    std::string Type() override
    {
        return "LessThanOrEqual";
    }
};