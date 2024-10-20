#include "not_expression.hh"

NotExpression::NotExpression(std::shared_ptr<IExpressionable> exp_in, yy::location loc)
    : OneOperandExpression(exp_in,loc) {}

std::shared_ptr<Type> NotExpression::GetType()
{
    return std::make_shared<TypeBoolean>();
}

bool NotExpression::Check()
{
    if (!expression->Check()) 
    {
        return false;
    }

    auto exp_type = expression->GetType();

    if (exp_type->NotCompatible(std::make_shared<TypeBoolean>())) 
    {
        Error::ShowError(Helper::FormatString(
            "The expression of not expression must be of type boolean! (received '%s')",
            exp_type->ToString().c_str()
        ), expression->location);
        return false;
    }

    return true;
}
