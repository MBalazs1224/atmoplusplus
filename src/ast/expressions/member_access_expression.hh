#pragma once
#include "two_operand_expression.hh"
#include "../nodes/member_container.hh"
#include "identifier.hh"
class MemberAccessExpression : public TwoOperandExpression, public MemberContainer
{

    private:
        // The left part of the member access expression must be an identifier, so we store it here so we don't have to cast it every time
        std::shared_ptr<Identifier> identifier;

    public:
    MemberAccessExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc);

    std::shared_ptr<Type> GetType() override;

    ~MemberAccessExpression() override = default;

    bool Check() override;
    std::shared_ptr<SymbolTableElement> GetElement() override;

    std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;

    std::shared_ptr<IRStatement> TranslateToIR() override;

};
