#pragma once
#include "../location/ilocation.hh"
#include "../ast/types/type.hh"
#include "../ast/attributes/attribute.hh"
#include <memory>
class SymbolTableElement : public ILocation
{
    private:
        std::unique_ptr<Type> type;
        std::unique_ptr<Attribute> attribute;
    public:
        SymbolTableElement()
        {
        }
        void SetType(std::unique_ptr<Type>);
        void SetAttribute(std::unique_ptr<Attribute>);
};