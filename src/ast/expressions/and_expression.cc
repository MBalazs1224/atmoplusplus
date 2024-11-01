#include "and_expression.hh"


bool AndExpression::Check()
{
    if(!left->Check() || !right->Check())
    {
        return false;
    }
    auto boolean = Helper::BooleanType;
    auto left_type = left->GetType();
    auto right_type = right->GetType();
    if (left_type->NotCompatible(boolean) || right_type->NotCompatible(boolean))
    {
        Error::ShowError(Helper::FormatString("Both of the two operands of AND (&&) expression must be of type boolean! (received '%s' and '%s')",left_type->ToString().c_str(),right_type->ToString().c_str()),location);
        return false;
    }
    return true;
}

std::shared_ptr<Type> AndExpression::GetType()
{
    return Helper::BooleanType;
}

AndExpression::AndExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : TwoOperandExpression(left_in,right_in,loc)
{
}


AndExpression::~AndExpression() = default;