#include "symbolfunction.hh"

FunctionSymbol::FunctionSymbol(std::shared_ptr<Type> type_in, std::shared_ptr<Attribute> attr_in, std::vector<std::shared_ptr<VariableSymbol>> args_in, std::unique_ptr<BodyNode> body_in)
    : SymbolTableElement(type_in, std::move(attr_in)),body(std::move(body_in)), arguments(std::move(args_in))  {}

FunctionSymbol::FunctionSymbol(std::shared_ptr<Attribute> attr_in, std::unique_ptr<BodyNode> body_in, std::vector<std::shared_ptr<VariableSymbol>> args_in) : SymbolTableElement(std::move(attr_in)), body(std::move(body_in)), arguments(std::move(args_in)) {}


std::shared_ptr<Type> FunctionSymbol::GetType() {
    if (auto id = std::dynamic_pointer_cast<Identifier>(type)) {
        return id->GetType();
    }
    return this->type;
}

bool FunctionSymbol::Check() {
    // TODO: Implement symbol function checking
    return body->Check();
}

const std::vector<std::shared_ptr<VariableSymbol>>& FunctionSymbol::GetArguments() {
    return arguments;
}
