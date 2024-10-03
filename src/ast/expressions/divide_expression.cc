#include "divide_expression.hh"
bool DivideExpression::Check()
{
    if(!exp_left->Check() || !exp_right->Check())
    {
        return false;
    }
    auto exp_left_type = exp_left->GetType();
    auto exp_right_type = exp_right->GetType();
    if (exp_left_type->NotCompatible(exp_right_type))
    {
        Error::ShowError(Helper::FormatString("The two operands of DIVIDE (/) expression must be of same type! (received '%s' and '%s')",exp_left_type->ToString().c_str(),exp_right_type->ToString().c_str()),location);
        return false;
    }
    return true;
}

DivideExpression::DivideExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : IExpressionable(loc), exp_left(left_in), exp_right(right_in)
{
}
std::shared_ptr<Type> DivideExpression::GetType()
{
    return std::make_shared<TypeFloat>();
}