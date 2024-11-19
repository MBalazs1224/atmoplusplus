#include "array_subscript.hh"

ArraySubscriptExpression::ArraySubscriptExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : TwoOperandExpression(left_in,right_in,loc)
{
}

std::shared_ptr<Type> ArraySubscriptExpression::GetType()
{
    // At this point the Check funtion checked that the left expression is an array so we can just cast it

    auto array = std::dynamic_pointer_cast<Array>(left->GetType());

    return array->inner_type;
}

bool ArraySubscriptExpression::Check()
{
    if(!left->Check())
    {
        return false;
    }

    if(!right->Check())
    {
        return false;
    }

    auto arrayType = left->GetType();
    if (!std::dynamic_pointer_cast<Array>(arrayType))
    {
        Error::ShowError(Helper::FormatString("Only array types can appear on the left side of array subscript expression! (received '%s')",arrayType->ToString().c_str()),left->location);
        return false;
    }

    auto intType = right->GetType();
    if (!std::dynamic_pointer_cast<TypeInteger>(intType))
    {
        Error::ShowError(Helper::FormatString("Only integer types can appear on the right side of array subscript expression! (received '%s')",intType->ToString().c_str()),right->location);
        return false;
    }

    return true;
}