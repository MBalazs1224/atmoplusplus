#pragma once
#include "../symboltableelement.hh"
class VariableSymbol : public SymbolTableElement
{
    public:
    VariableSymbol(std::shared_ptr<Type> type_in, std::unique_ptr<Attribute> attr_in) : SymbolTableElement(type_in,std::move(attr_in))
    {

    }
};
