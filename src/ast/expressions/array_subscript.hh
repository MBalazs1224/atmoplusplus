#pragma once
#include "two_operand_expression.hh"
#include <memory>
class ArraySubscriptExpression : public TwoOperandExpression
{
    public:
    ArraySubscriptExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    
    ~ArraySubscriptExpression() override = default;
    std::shared_ptr<Type> GetType() override;

    bool Check() override;
};