#pragma once
#include "two_operand_expression.hh"
#include "../nodes/member_container.hh"
class MemberAccessExpression : public TwoOperandExpression, public MemberContainer
{
    public:
    MemberAccessExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);

    std::shared_ptr<Type> GetType() override;

    ~MemberAccessExpression() override = default;

    bool Check() override;
    std::shared_ptr<SymbolTableElement> GetElement() override;
};
