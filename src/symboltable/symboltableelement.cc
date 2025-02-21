#include "symboltableelement.hh"

#include "../ast/attributes/all_attributes.hh"

SymbolTableElement::SymbolTableElement(std::shared_ptr<Type> type,std::vector<std::shared_ptr<Attribute>> attr)
{
    this->type = type;
    this->attributes = std::move(attr);
}

std::vector<std::shared_ptr<Attribute>> SymbolTableElement::GetAttributes()
{
    return attributes;
}

SymbolTableElement::~SymbolTableElement() = default;

bool SymbolTableElement::IsReachableOutside()
{
    auto publicAttribute = std::make_shared<AttributePublic>();

    // The element is reachable outside if it has a public attribute

    return std::find(attributes.begin(),attributes.end(),publicAttribute) == attributes.end();
}


SymbolTableElement::SymbolTableElement(std::vector<std::shared_ptr<Attribute>> attr)
{
    this->attributes = std::move(attr);
}