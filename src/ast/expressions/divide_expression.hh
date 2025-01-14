#pragma once
#include "two_operand_expression.hh"
#include <memory>
class DivideExpression : public TwoOperandExpression
{
    public:
    DivideExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);

    std::shared_ptr<Type> GetType() override;

    ~DivideExpression() override = default;

    bool Check() override;

    std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;

    std::shared_ptr<IRStatement> TranslateToIR() override;
};
