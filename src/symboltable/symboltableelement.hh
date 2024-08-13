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
        SymbolTableElement(std::unique_ptr<Type>,std::unique_ptr<Attribute>);
};