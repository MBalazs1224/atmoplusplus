#pragma once
#include "expressionable.hh"
#include <memory>
class AddExpression : public IExpressionable
{
    std::shared_ptr<IExpressionable> exp_left; 
    std::shared_ptr<IExpressionable> exp_right; 
    public:
    AddExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    
    ~AddExpression() override = default;
    std::shared_ptr<Type> GetType() override;

    bool Check() override;
};
