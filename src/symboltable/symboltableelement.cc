#include "symboltableelement.hh"

void SymbolTableElement::SetType(std::unique_ptr<Type> type)
{
    this->type = std::move(type);
}

void SymbolTableElement::SetAttribute(std::unique_ptr<Attribute> attr)
{
    this->attribute = std::move(attr);
}