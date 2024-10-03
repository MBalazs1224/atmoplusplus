#include "not_expression.hh"

NotExpression::NotExpression(std::shared_ptr<IExpressionable> exp_in, yy::location loc)
    : IExpressionable(loc), exp(std::move(exp_in)) {}

std::shared_ptr<Type> NotExpression::GetType()
{
    return std::make_shared<TypeBoolean>();
}

bool NotExpression::Check()
{
    if (!exp->Check()) 
    {
        return false;
    }

    auto exp_type = exp->GetType();

    if (exp_type->NotCompatible(std::make_shared<TypeBoolean>())) 
    {
        Error::ShowError(Helper::FormatString(
            "The expression of not expression must be of type boolean! (received '%s')",
            exp_type->ToString().c_str()
        ), exp->location);
        return false;
    }

    return true;
}
