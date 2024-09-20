#pragma once
#include "expressionable.hh"
class MatchesExpression : public IExpressionable
{
    std::shared_ptr<IExpressionable> exp_left;
	std::shared_ptr<IExpressionable> exp_right;
    public:
    MatchesExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    std::shared_ptr<Type> GetType() override;
    ~MatchesExpression() override = default;

    bool Check() override;
};