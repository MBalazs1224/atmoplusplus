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

    void Check() override
    {
        //TODO: Implement Or expression checking
    }
};