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

        // FIXME: Assignment might needit's unique type
        return exp_left->GetType();
    }
    bool Check() override
    {
        if (!exp_left->Check() || !exp_right->Check())
        {
            return false;
        }
        
        auto identifier = std::dynamic_pointer_cast<Identifier>(exp_left);
        if (!identifier)
        {
            Error::ShowError("Only variables can appear on the left side of an assignment (=) expression!",exp_left->location);
            return false;
        }

        auto variable = std::dynamic_pointer_cast<VariableSymbol>(identifier->GetElement());

        if (!variable)
        {
            Error::ShowError("Only variables can appear on the left side of an assignment (=) expression!",exp_left->location);
            return false;
        }
        

        auto variable_type = exp_left->GetType();
        auto exp_right_type = exp_right->GetType();
        if (variable_type->NotEquals(exp_right_type))
        {
            Error::ShowError(Error::FormatString("Invalid type of expression on the right side of assignment (=) expression! The expression's type ('%s') must match the variable's type ('%s')!",exp_right_type->ToString().c_str(),variable_type->ToString().c_str()),exp_right->location);
            return false;
        }
        return true;
    }
};
