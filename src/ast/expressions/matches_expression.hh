#pragma once
#include "two_operand_expression.hh"
class MatchesExpression : public TwoOperandExpression
{
    std::shared_ptr<IExpressionable> left;
	std::shared_ptr<IExpressionable> right;
    public:
    MatchesExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    std::shared_ptr<Type> GetType() override;
    ~MatchesExpression() override = default;

    bool Check() override;

    std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;

    std::shared_ptr<IRStatement> TranslateToIR() override;

};