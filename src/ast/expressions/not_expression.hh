#pragma once
#include "expressionable.hh"
class NotExpression : public IExpressionable
{
    std::shared_ptr<IExpressionable> exp;
    public:
    NotExpression(std::shared_ptr<IExpressionable> exp_in,yy::location loc) : IExpressionable(loc), exp(exp_in)
    {

    }
    std::shared_ptr<Type> GetType() override
    {
        return std::make_shared<TypeBoolean>();
    }
    ~NotExpression() override = default;
    bool Check() override
    {
        if (!exp->Check())
        {
            return false;
        }

        // The NOT statement can only work with boolean types
        auto exp_type = exp->GetType();
        if (exp_type->NotEquals(std::make_shared<TypeBoolean>()))
        {
            Error::ShowError(Error::FormatString("The expression of not expression must be of type boolean! (received '%s')",exp_type->ToString().c_str()),exp->location);
            return false;
        }
        
        
        return true;
    }
};