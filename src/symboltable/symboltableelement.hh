#pragma once
#include "../location/ilocation.hh"
#include "../ast/types/type.hh"
class SymbolTableElement : public ILocation
{
    private:
        Type type;

    public:
        SymbolTableElement()
        {
        }
        void SetType(Type);
};