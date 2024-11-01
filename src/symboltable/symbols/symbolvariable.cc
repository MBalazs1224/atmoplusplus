#include "symbolvariable.hh"

VariableSymbol::VariableSymbol(std::shared_ptr<Type> type_in, std::shared_ptr<Attribute> attr_in)
    : SymbolTableElement(type_in, std::move(attr_in)) {}

std::shared_ptr<Type> VariableSymbol::GetType() {
    if (auto id = std::dynamic_pointer_cast<Identifier>(type)) {
        return id->GetType();
    }
    return this->type;
}

bool VariableSymbol::Check() {
    return true;
}
