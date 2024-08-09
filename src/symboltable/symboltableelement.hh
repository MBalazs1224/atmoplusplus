#pragma once
#include "../location/ilocation.hh"
#include "../ast/types/type.hh"
#include "../ast/attributes/attribute.hh"
class SymbolTableElement : public ILocation
{
    private:
        Type type;
        Attribute attribute;
    public:
        SymbolTableElement()
        {
        }
        void SetType(Type);
        void SetAttribute(Attribute);
};