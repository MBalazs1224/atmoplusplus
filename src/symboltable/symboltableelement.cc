#include "symboltableelement.hh"

SymbolTableElement::SymbolTableElement(Type type,std::shared_ptr<Attribute> attr)
{
    this->type = type;
    this->attribute = std::move(attr);
}

SymbolTableElement::~SymbolTableElement() = default;