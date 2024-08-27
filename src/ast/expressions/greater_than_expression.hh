#pragma once
#include "expressionable.hh"
class GreaterThanExpression : public IExpressionable
{
	std::shared_ptr<IExpressionable> exp_left;  
	std::shared_ptr<IExpressionable> exp_right;
    public:
    GreaterThanExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : IExpressionable(loc), exp_left(left_in), exp_right(right_in)
    {

    }
    ~GreaterThanExpression() override = default;
};
