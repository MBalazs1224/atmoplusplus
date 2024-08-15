#pragma once
#include "expression.hh"
class AddExpression : public Expression
{
    
    public:
    AddExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in) : Expression(std::move(left_in),std::move(right_in))
    {

    }
    ~AddExpression() override = default;
    std::string Type() override
    {
        return "Add";
    }

};