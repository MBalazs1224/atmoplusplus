#pragma once
#include "expressionable.hh"
class LessThanOrEqualExpression : public IExpressionable
{
    std::shared_ptr<IExpressionable> exp_left;
	std::shared_ptr<IExpressionable> exp_right;
    public:
    LessThanOrEqualExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);
    std::shared_ptr<Type> GetType() override;
    ~LessThanOrEqualExpression() override = default;
    bool Check() override;
};