#include "one_operand_expression.hh"

OneOperandExpression::OneOperandExpression(std::shared_ptr<IExpressionable> expression, yy::location loc) : IExpressionable(loc), expression(expression) {}


std::shared_ptr<IExpressionable> OneOperandExpression::GetExpression()
{
    return expression;
}