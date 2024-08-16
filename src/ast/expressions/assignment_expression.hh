#pragma once
#include "expressionable.hh"
#include "expression.hh"
#include <memory>
#include <iostream>

class AssignmentExpression : public Expression
{


    public:
        AssignmentExpression(std::shared_ptr<IExpressionable> var, std::shared_ptr<IExpressionable> exp_in,yy::location loc) : Expression(var,exp_in,loc)
        {

        }
};