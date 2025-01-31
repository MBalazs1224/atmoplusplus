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
    // Use the access object to retreive this variable's location

    auto fp_exp = std::make_shared<IRTemp>(ReservedIrRegisters::StackPointer);
    auto access_exp = access->AsExpression(fp_exp);
    
    return std::make_shared<TranslateValueExpression>(access_exp);
}

std::shared_ptr<IRStatement> VariableSymbol::TranslateToIR()
{
    throw std::runtime_error("VariableSymbol should not be translated to IR");
}
