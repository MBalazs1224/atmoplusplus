#pragma once
#include "expressionable.hh"

// Parent class for expressions that have two operands
class TwoOperandExpression : public IExpressionable
{
public:
    // The left operand of the expression
    std::shared_ptr<IExpressionable> left;
    // The right operand of the expression
    std::shared_ptr<IExpressionable> right;

    TwoOperandExpression(std::shared_ptr<IExpressionable> left, std::shared_ptr<IExpressionable> right, yy::location loc);

    // Returns the left operand of the expression
    std::shared_ptr<IExpressionable> GetLeft() const;

    // Returns the right operand of the expression
    std::shared_ptr<IExpressionable> GetRight() const;


};