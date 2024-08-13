#pragma once
#include "../symboltableelement.hh"
class VariableSymbol : public SymbolTableElement
{
    public:
    VariableSymbol(std::unique_ptr<Type> type_in, std::unique_ptr<Attribute> attr_in) : SymbolTableElement(std::move(type_in),std::move(attr_in))
    {

    }
};
