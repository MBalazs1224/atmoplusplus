#pragma once
#include "expressionable.hh"

// Parent class for expressions that have two operands
class OneOperandExpression : public IExpressionable
{
public:
    // The  operand of the expression
    std::shared_ptr<IExpressionable> expression;

    OneOperandExpression(std::shared_ptr<IExpressionable>, yy::location);

    // Returns the operand of the expression
    std::shared_ptr<IExpressionable> GetExpression();


};