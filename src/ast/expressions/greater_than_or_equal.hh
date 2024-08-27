#pragma once
#include "expressionable.hh"
#include <memory>
class GreaterThanOrEqualExpression : public IExpressionable    
{
	std::shared_ptr<IExpressionable> exp_left;
	std::shared_ptr<IExpressionable> exp_right;
    public:
    GreaterThanOrEqualExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : IExpressionable(loc), exp_left(left_in), exp_right(right_in)
    {

    }
    ~GreaterThanOrEqualExpression() override = default;
};
