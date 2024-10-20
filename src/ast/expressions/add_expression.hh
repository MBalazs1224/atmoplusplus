#pragma once
#include "two_operand_expression.hh"
#include <memory>
class AddExpression : public TwoOperandExpression
{
    public:
    AddExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    
    ~AddExpression() override = default;
    std::shared_ptr<Type> GetType() override;

    bool Check() override;
};
