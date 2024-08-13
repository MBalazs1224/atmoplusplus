#include "symboltableelement.hh"

SymbolTableElement::SymbolTableElement(std::unique_ptr<Type> type,std::unique_ptr<Attribute> attr)
{
    this->type = std::move(type);
    this->attribute = std::move(attr);
}