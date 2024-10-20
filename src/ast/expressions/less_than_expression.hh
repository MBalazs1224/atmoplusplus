#pragma once
#include "two_operand_expression.hh"
class LessThanExpression : public TwoOperandExpression
{
    public:
    LessThanExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    std::shared_ptr<Type> GetType() override;
    ~LessThanExpression() override = default;
    bool Check() override;
};