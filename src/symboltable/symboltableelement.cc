#include "symboltableelement.hh"

SymbolTableElement::SymbolTableElement(std::shared_ptr<Type> type,std::unique_ptr<Attribute> attr)
{
    this->type = type;
    this->attribute = std::move(attr);
}

SymbolTableElement::~SymbolTableElement() = default;