#pragma once
#include "expressionable.hh"
#include <memory>
#include <iostream>

class AssignmentExpression : public IExpressionable
{
	std::shared_ptr<IExpressionable> exp_left;
	std::shared_ptr<IExpressionable> exp_right;

    public:
        AssignmentExpression(std::shared_ptr<IExpressionable> exp_left_in, std::shared_ptr<IExpressionable> exp_right_in,yy::location loc) : IExpressionable(loc), exp_left(exp_left_in),exp_right(exp_right_in)
        {

        }
};
