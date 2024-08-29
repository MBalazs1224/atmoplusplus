#pragma once
#include "expressionable.hh"
class MemberAccessExpression : public IExpressionable
{
    std::shared_ptr<IExpressionable> exp_left;
	std::shared_ptr<IExpressionable> exp_right;
    public:
    MemberAccessExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : IExpressionable(loc), exp_left(left_in),exp_right(right_in)
    {

    }
        std::shared_ptr<Type> GetType() override
    {
        // FIXME: Member access might needs it's unique type
        return exp_left->GetType();
    }
    ~MemberAccessExpression() override = default;
};
