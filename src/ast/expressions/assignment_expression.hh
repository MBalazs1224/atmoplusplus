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
    std::shared_ptr<Type> GetType() override
    {
        // TODO: We need to check if the type of the two expression match

        // FIXME: Assignment might needit's unique type
        return exp_left->GetType();
    }
    bool Check() override
    {
        //TODO: Implement Assignment expression checking
        return true;
    }
};
