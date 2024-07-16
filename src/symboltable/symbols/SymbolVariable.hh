#pragma once
#include "../SymbolTableElement.hh"
class VariableSymbol : public SymbolTableElement
{
    public:
    VariableSymbol(int lineno, int length) : SymbolTableElement(lineno,length)
    {
        
    }
};