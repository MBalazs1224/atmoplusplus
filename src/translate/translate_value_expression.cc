#include "translate_value_expression.hh"
#include <stdexcept>
TranslateValueExpression::TranslateValueExpression(std::shared_ptr<IRExpression> exp_in) : exp(std::move(exp_in))
{
}


std::shared_ptr<IRExpression> TranslateValueExpression::ToValueExpression()
{
    return exp;
}

std::shared_ptr<IRStatement> TranslateValueExpression::ToNoValueExpression()
{
    // If we don't want it's value, we can just evaluate it

    return std::make_shared<IREvaluateExpression>(exp);
}

std::shared_ptr<IRStatement> TranslateValueExpression::ToConditionExpression(std::shared_ptr<Label> trueLabel, std::shared_ptr<Label> flaseLabel)
{
    // a = 5 or a + 3 for example cannot be used as a condition expression

    throw std::logic_error("Cannot convert value expression to condition expression, this should not happen");
}


