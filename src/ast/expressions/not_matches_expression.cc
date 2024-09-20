#include "not_matches_expression.hh"

NotMatchesExpression::NotMatchesExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in, yy::location loc)
    : IExpressionable(loc), exp_left(std::move(left_in)), exp_right(std::move(right_in)) {}

std::shared_ptr<Type> NotMatchesExpression::GetType() 
{
    return std::make_shared<TypeBoolean>();
}

bool NotMatchesExpression::Check() 
{
    // TODO: Implement Not matches expression checking
    return true;
}
