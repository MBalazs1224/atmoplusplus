#pragma once
#include "../symboltableelement.hh"
class VariableSymbol : public SymbolTableElement
{
    public:
    VariableSymbol(std::unique_ptr<Type> type, std::unique_ptr<Attribute> attr) : SymbolTableElement(std::move(type),std::move(attr))
    {

    }
};
