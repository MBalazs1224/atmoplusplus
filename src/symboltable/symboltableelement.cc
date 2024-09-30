#include "symboltableelement.hh"

#include "../ast/attributes/all_attributes.hh"

SymbolTableElement::SymbolTableElement(std::shared_ptr<Type> type,std::shared_ptr<Attribute> attr)
{
    this->type = type;
    this->attribute = std::move(attr);
}

std::shared_ptr<Attribute> SymbolTableElement::GetAttribute()
{
    return attribute;
}

SymbolTableElement::~SymbolTableElement() = default;

bool SymbolTableElement::IsReachableOutside()
{
    return std::dynamic_pointer_cast<AttributePublic>(attribute) || std::dynamic_pointer_cast<AttributeStatic>(attribute);
}


SymbolTableElement::SymbolTableElement(std::shared_ptr<Attribute> attr)
{
    this->attribute = std::move(attr);
}