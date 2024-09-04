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
    void Check() override
    {
        //TODO: Implement Not expression checking
    }
};