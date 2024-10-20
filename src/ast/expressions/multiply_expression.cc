#include "multiply_expression.hh"

MultiplyExpression::MultiplyExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in, yy::location loc)
    : TwoOperandExpression(left_in,right_in,loc) {}

std::shared_ptr<Type> MultiplyExpression::GetType()
{
    return left->GetType();
}

bool MultiplyExpression::Check() {
    if (!left->Check() || !right->Check())
    {
        return false;
    }

    auto left_type = left->GetType();
    auto right_type = right->GetType();

    if (left_type->NotCompatible(right_type))
    {
        Error::ShowError(Helper::FormatString(
            "The two operands of MULTIPLY (*) expression must be of same type! (received '%s' and '%s')",
            left_type->ToString().c_str(),
            right_type->ToString().c_str()
        ), location);
        return false;
    }

    return true;
}
