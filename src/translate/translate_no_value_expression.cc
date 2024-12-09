#include "translate_no_value_expression.hh"
#include <stdexcept>
TranslateNoValueExpression::TranslateNoValueExpression(std::shared_ptr<IRStatement> stmt_in) : stmt(std::move(stmt_in))
{
}

std::shared_ptr<IRStatement> TranslateNoValueExpression::ToNoValueExpression()
{
    return stmt;
}

std::shared_ptr<IRExpression> TranslateNoValueExpression::ToValueExpression()
{
    //  A statement that doesn't have a value cannot be converted to a value expression

    throw std::logic_error("Cannot convert no value expression to value expression, this should not happen");
}


std::shared_ptr<IRStatement> TranslateNoValueExpression::ToConditionExpression(std::shared_ptr<Label> trueLabel, std::shared_ptr<Label> flaseLabel)
{
    // A statement that doesn't have a value cannot be converted to a condition expression

    throw std::logic_error("Cannot convert no value expression to condition expression, this should not happen");
}