#include "two_operand_expression.hh"

TwoOperandExpression::TwoOperandExpression(std::shared_ptr<IExpressionable> left, std::shared_ptr<IExpressionable> right,yy::location loc) : IExpressionable(loc), left(left), right(right) {}

    // Returns the left operand of the expression
std::shared_ptr<IExpressionable> TwoOperandExpression::GetLeft() const
{
    return left;
}

    // Returns the right operand of the expression
std::shared_ptr<IExpressionable> TwoOperandExpression::GetRight() const
{
    return right;
}
        