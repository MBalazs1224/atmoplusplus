#pragma once
#include "one_operand_expression.hh"
class NotExpression : public OneOperandExpression
{
    public:
    NotExpression(std::shared_ptr<IExpressionable> exp_in,yy::location loc);
    std::shared_ptr<Type> GetType() override;
    ~NotExpression() override = default;
    bool Check() override;
};