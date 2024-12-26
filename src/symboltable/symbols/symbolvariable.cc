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

DataSize VariableSymbol::GetSize()
{
    return type->GetSize();
}

std::shared_ptr<TranslateExpression> VariableSymbol::TranslateExpressionToIr()
{
    throw std::runtime_error("VariableSymbol should not be translated to IR as expression");
}

std::shared_ptr<IRStatement> VariableSymbol::TranslateToIR()
{
    throw std::runtime_error("VariableSymbol should not be translated to IR");
}
