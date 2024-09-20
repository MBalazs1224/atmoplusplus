#pragma once
#include "expressionable.hh"
#include <memory>
#include <iostream>

class AssignmentExpression : public IExpressionable
{
	std::shared_ptr<IExpressionable> exp_left;
	std::shared_ptr<IExpressionable> exp_right;

    public:
        AssignmentExpression(std::shared_ptr<IExpressionable> exp_left_in, std::shared_ptr<IExpressionable> exp_right_in,yy::location loc);
        
        std::shared_ptr<Type> GetType() override;

        bool Check() override;
};
