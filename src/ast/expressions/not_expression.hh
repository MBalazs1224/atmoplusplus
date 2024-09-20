#pragma once
#include "expressionable.hh"
class NotExpression : public IExpressionable
{
    std::shared_ptr<IExpressionable> exp;
    public:
    NotExpression(std::shared_ptr<IExpressionable> exp_in,yy::location loc);
    std::shared_ptr<Type> GetType() override;
    ~NotExpression() override = default;
    bool Check() override;
};