#include "and_expression.hh"


bool AndExpression::Check()
{
    if(!exp_left->Check() || !exp_right->Check())
    {
        return false;
    }   // FIXME: Fix boolean dynamic memory alloc
    auto boolean = std::make_shared<TypeBoolean>();
    auto exp_left_type = exp_left->GetType();
    auto exp_right_type = exp_right->GetType(); if (exp_left_type->NotCompatible(boolean) || exp_right_type->NotCompatible(boolean))
    {
        Error::ShowError(Helper::FormatString("Both of the two operands of AND (&&) expression must be of type boolean! (received '%s' and '%s')",exp_left_type->ToString().c_str(),exp_right_type->ToString().c_str()),location);
        return false;
    }
    return true;
}

std::shared_ptr<Type> AndExpression::GetType()
{
    return std::make_shared<TypeBoolean>();
}

AndExpression::AndExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : IExpressionable(loc), exp_left(std::move(left_in)),exp_right(std::move(right_in))
{
}


AndExpression::~AndExpression() = default;