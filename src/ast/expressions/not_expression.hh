#pragma once
#include "expressionable.hh"
class NotExpression : public IExpressionable
{
    std::shared_ptr<IExpressionable> exp;
    public:
    NotExpression(std::shared_ptr<IExpressionable> exp_in,yy::location loc) : IExpressionable(loc), exp(exp_in)
    {

    }
    ~NotExpression() override = default;
};