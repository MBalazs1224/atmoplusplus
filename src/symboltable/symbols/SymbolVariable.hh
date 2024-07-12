#pragma once
#include "../SymbolTableElement.hh"
class VariableSymbol : public SymbolTableElement
{
    public:
    VariableSymbol(int lineno, int length, int scope) : SymbolTableElement(lineno,length,scope)
    {
        
    }
};