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
    // To convert a value expression to a condition expression we need to compare it to 0 (if it not equals no 0, than it means the value is true, otherwise false)

    auto zeroExpression = std::make_shared<IRConst>(0);

    auto condition = std::make_shared<IRCJump>(RelationalOperator::NOTEQUALS, exp, zeroExpression, trueLabel, flaseLabel);

    return condition;
}


