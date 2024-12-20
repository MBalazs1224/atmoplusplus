#pragma once
#include "two_operand_expression.hh"
#include <memory>
class AndExpression : public TwoOperandExpression
{
    public:
    AndExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    
    ~AndExpression() override;

    std::shared_ptr<Type> GetType() override;

    bool Check() override;

    std::shared_ptr<TranslateExpression> TranslateExpressionToIr();
};
