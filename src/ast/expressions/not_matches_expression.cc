#include "not_matches_expression.hh"

NotMatchesExpression::NotMatchesExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in, yy::location loc)
    : IExpressionable(loc), exp_left(std::move(left_in)), exp_right(std::move(right_in)) {}

std::shared_ptr<Type> NotMatchesExpression::GetType() 
{
    return std::make_shared<TypeBoolean>();
}

bool NotMatchesExpression::Check() 
{
    if (!exp_left->Check() || !exp_right->Check())
    {
        return false;
    }
    auto exp_left_type = exp_left->GetType();
    auto exp_right_type = exp_right->GetType();

    if (exp_left_type->NotEquals(exp_right_type))
    {
        Error::ShowError(Helper::FormatString("The two operands of NOT MATCHES (!=) expression must be of same type! (received '%s' and '%s')", exp_left_type->ToString().c_str(),exp_right_type->ToString().c_str()),this->location);
        return false;
    }

    return true;
}
