#pragma once
#include "two_operand_expression.hh"
class SubtractExpression : public TwoOperandExpression
{
    public:
    SubtractExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    std::shared_ptr<Type> GetType() override;
    ~SubtractExpression() override = default;

    bool Check() override;

    std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;
};