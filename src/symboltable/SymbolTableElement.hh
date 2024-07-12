#pragma once
class SymbolTableElement
{
    protected:
        int lineno;
        int length;
        int scope;
    public:
        SymbolTableElement(int lineno, int length, int scope)
        {
            this->lineno = lineno;
            this->length = length;
            this->scope = scope;
        }
};