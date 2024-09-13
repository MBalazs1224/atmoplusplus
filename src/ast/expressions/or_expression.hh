#pragma once
#include "expressionable.hh"
class OrExpression : public IExpressionable
{
    std::shared_ptr<IExpressionable> exp_left;
	std::shared_ptr<IExpressionable> exp_right;
    public:
    OrExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : IExpressionable(loc), exp_left(left_in),exp_right(right_in)
    {

    }
    std::shared_ptr<Type> GetType() override
    {
        // TODO: Need to check if the type of the 2 expressions match
        return std::make_shared<TypeBoolean>();
    }
    ~OrExpression() override = default;

    bool Check() override
    {
        if(!exp_left->Check() || !exp_right->Check())
        {
            return false;
        }

        // FIXME: Fix boolean dynamic memory alloc
        auto boolean = std::make_shared<TypeBoolean>();
        auto exp_left_type = exp_left->GetType();
        auto exp_right_type = exp_right->GetType();

        if (exp_left_type->NotEquals(boolean) || exp_right_type->NotEquals(boolean))
        {
            Error::ShowError(Error::FormatString("Both of the two operands of OR (||) expression must be of type boolean! (received '%s' and '%s')",exp_left_type->ToString().c_str(),exp_right_type->ToString().c_str()),location);
            return false;
        }
        return true;

    }
};