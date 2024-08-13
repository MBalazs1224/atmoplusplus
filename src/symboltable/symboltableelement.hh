#pragma once
#include "../location/ilocation.hh"
#include "../ast/types/type.hh"
#include "../ast/attributes/attribute.hh"
#include "../ast/attributes/attributeprivate.hh"
#include <memory>
class SymbolTableElement : public ILocation
{
    protected:
    // FIXME: These definitely shouldn't be a pointer
        std::unique_ptr<Type> type;
        std::unique_ptr<Attribute> attribute = std::make_unique<AttributePrivate>();
    public:
        SymbolTableElement()
        {
        }
        SymbolTableElement(std::unique_ptr<Type>,std::unique_ptr<Attribute>);
        virtual ~SymbolTableElement();
};