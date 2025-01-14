#pragma once
#include "two_operand_expression.hh"
class LessThanOrEqualExpression : public TwoOperandExpression
{
    public:
    LessThanOrEqualExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    std::shared_ptr<Type> GetType() override;
    ~LessThanOrEqualExpression() override = default;
    bool Check() override;

    std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;

    std::shared_ptr<IRStatement> TranslateToIR() override;

};