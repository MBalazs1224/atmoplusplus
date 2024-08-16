#pragma once
#include "expression.hh"
class GreaterThanOrEqualExpression : public Expression
{

    public:
    GreaterThanOrEqualExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : Expression(std::move(left_in),std::move(right_in),loc)
    {

    }
    ~GreaterThanOrEqualExpression() override = default;
};