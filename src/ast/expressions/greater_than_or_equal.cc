#include "greater_than_or_equal.hh"
bool GreaterThanOrEqualExpression::Check()
{
    if(!exp_left->Check() || !exp_right->Check())
    {
        return false;
    }
    auto exp_left_type = exp_left->GetType();
    auto exp_right_type = exp_right->GetType();
    if (exp_left_type->NotEquals(exp_right_type))
    {
        Error::ShowError(Helper::FormatString("The two operands of GREATER THAN OR EQUAL (>=) expression must be of same type! (received '%s' and '%s')",exp_left_type->ToString().c_str(),exp_right_type->ToString().c_str()),location);
        return false;
    }
    return true;
}

std::shared_ptr<Type> GreaterThanOrEqualExpression::GetType()
{
    return std::make_shared<TypeBoolean>();
}

GreaterThanOrEqualExpression::GreaterThanOrEqualExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : IExpressionable(loc), exp_left(left_in), exp_right(right_in)
{
}