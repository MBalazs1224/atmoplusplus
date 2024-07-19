#pragma once
#include "../symboltableelement.hh"
class VariableSymbol : public SymbolTableElement
{
    public:
    VariableSymbol(int lineno, int length) : SymbolTableElement(lineno,length)
    {
        
    }
};
