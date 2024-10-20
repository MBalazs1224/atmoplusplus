#pragma once
#include "two_operand_expression.hh"
class GreaterThanExpression : public TwoOperandExpression
{

    public:
        GreaterThanExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    
        std::shared_ptr<Type> GetType() override;
    
        ~GreaterThanExpression() override = default;
    
        bool Check() override;
};
