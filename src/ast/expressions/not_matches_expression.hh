#pragma once
#include "two_operand_expression.hh"
class NotMatchesExpression : public TwoOperandExpression
{
    public:
    NotMatchesExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    std::shared_ptr<Type> GetType() override;
    ~NotMatchesExpression() override = default;

    bool Check() override;

    std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;

    std::shared_ptr<IRStatement> TranslateToIR() override;

};