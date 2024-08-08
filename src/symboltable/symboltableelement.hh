#pragma once
#include "../location/ilocation.hh"
class SymbolTableElement : public ILocation
{
    protected:
        int lineno;
        int length;
        int scope;
    public:
        SymbolTableElement(int lineno, int length)
        {
            this->lineno = lineno;
            this->length = length;
        }
        void AddScope(int scope)
        {
            this->scope = scope;
        }
};