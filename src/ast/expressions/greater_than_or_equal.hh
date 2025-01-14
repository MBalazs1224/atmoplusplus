#pragma once
#include "two_operand_expression.hh"
#include <memory>
class GreaterThanOrEqualExpression : public TwoOperandExpression    
{

    public:
        GreaterThanOrEqualExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);

        std::shared_ptr<Type> GetType() override;

        ~GreaterThanOrEqualExpression() override = default;

        bool Check() override;

        std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;

        std::shared_ptr<IRStatement> TranslateToIR() override;

};
