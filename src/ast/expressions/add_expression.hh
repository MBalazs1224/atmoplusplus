#pragma once
#include "expressionable.hh"
#include <memory>
class AddExpression : public IExpressionable
{
    std::shared_ptr<IExpressionable> exp_left; 
    std::shared_ptr<IExpressionable> exp_right; 
    public:
    AddExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : IExpressionable(loc), exp_left(std::move(left_in)),exp_right(std::move(right_in))
    {

    }
    ~AddExpression() override = default;
    std::shared_ptr<Type> GetType() override
    {
        // TODO: We need to check if the type of the two expression match
        return exp_left->GetType();
    }
};
