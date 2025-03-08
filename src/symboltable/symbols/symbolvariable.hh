#pragma once
#include "../symboltableelement.hh"
#include "../../ast/expressions/identifier.hh"
#include "../../frame/access.hh"
#include "../../ast/attributes/all_attributes.hh"
#include "class_member.hh"
class VariableSymbol : public SymbolTableElement, public ClassMember
{
    public:
    VariableSymbol(std::shared_ptr<Type> type_in, std::vector<std::shared_ptr<Attribute>> attr_in);
    std::shared_ptr<Type> GetType() override;
    bool Check() override;

    DataSize GetSize() override;


    std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;

    std::shared_ptr<TranslateExpression> TranslateExpressionToIrNoDereference() override;
    std::shared_ptr<IRStatement> TranslateToIR() override;
};
