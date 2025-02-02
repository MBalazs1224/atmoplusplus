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
    // To convert a value expression to a condition expression we need to compare it to 1 (if equals means true, otherwise false)

    auto zeroExpression = std::make_shared<IRConst>(1);

    auto condition = std::make_shared<IRCJump>(RelationalOperator::Matches, 
        exp, 
        zeroExpression, 
        trueLabel, 
        flaseLabel
    );

    return condition;
}


