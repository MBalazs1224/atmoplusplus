#pragma once
#include "two_operand_expression.hh"
#include "member_access_expression.hh"
#include <memory>
#include <iostream>


class AssignmentExpression : public TwoOperandExpression
{

    public:
        AssignmentExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
        
        std::shared_ptr<Type> GetType() override;

        bool Check() override;

        std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;

        std::shared_ptr<IRStatement> TranslateToIR() override;

};
