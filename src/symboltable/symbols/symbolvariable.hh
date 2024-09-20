#pragma once
#include "../symboltableelement.hh"
#include "../../ast/expressions/identifier.hh"
class VariableSymbol : public SymbolTableElement
{
    public:
    VariableSymbol(std::shared_ptr<Type> type_in, std::shared_ptr<Attribute> attr_in);
    std::shared_ptr<Type> GetType() override;
    bool Check() override;
};
