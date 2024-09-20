#pragma once
#include "../location/ilocation.hh"
#include "../ast/types/type.hh"
#include "../ast/attributes/attribute.hh"
#include "../ast/attributes/attributeprivate.hh"
#include "../ast/expressions/expressionable.hh"
#include <memory>
class SymbolTableElement : public IExpressionable
{
    protected:
    // These need to be pointers so typeid works correctly
        std::shared_ptr<Type> type;
        std::shared_ptr<Attribute> attribute = std::make_unique<AttributePrivate>();
    public:
        std::string name;
        SymbolTableElement()
        {
        }
        SymbolTableElement(std::shared_ptr<Type>,std::shared_ptr<Attribute>);
        virtual ~SymbolTableElement();
        std::shared_ptr<Attribute> GetAttribute();
        // Returns true if the element is static or public. (meaning its reachable outside the class)
        virtual bool IsReachableOutside();
};