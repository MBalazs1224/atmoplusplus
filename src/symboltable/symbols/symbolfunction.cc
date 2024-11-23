#include "symbolfunction.hh"

FunctionSymbol::FunctionSymbol(std::shared_ptr<Type> type_in, std::shared_ptr<Attribute> attr_in, std::vector<std::shared_ptr<VariableSymbol>> args_in, std::unique_ptr<BodyNode> body_in)
    : SymbolTableElement(type_in, std::move(attr_in)), arguments(std::move(args_in)),body(std::move(body_in))  {}

FunctionSymbol::FunctionSymbol(std::shared_ptr<Attribute> attr_in, std::unique_ptr<BodyNode> body_in, std::vector<std::shared_ptr<VariableSymbol>> args_in) : SymbolTableElement(std::move(attr_in)), arguments(std::move(args_in)), body(std::move(body_in)) {}

FunctionSymbol::FunctionSymbol(std::unique_ptr<BodyNode> body_in) : SymbolTableElement(nullptr), arguments(), body(std::move(body_in)) {}

std::shared_ptr<Type> FunctionSymbol::GetType() {
    if (auto id = std::dynamic_pointer_cast<Identifier>(type)) {
        return id->GetType();
    }
    return this->type;
}

bool FunctionSymbol::Check() {
    if(arguments.size() > 6)
    {
        Error::ShowError("Functions cannot have  more than 6 arguments!",this->location);
        return false;
    }
    if(!body->Check())
    {
        return false;
    }

    // Get all the variables
    local_variables = body->GetVariables();

    // Calculate the size of the function
    for (auto &&variable : local_variables)
    {
        this->size_in_bytes += variable->GetSize();
    }
    

    return true;
}

const std::vector<std::shared_ptr<VariableSymbol>>& FunctionSymbol::GetArguments() {
    return arguments;
}

FunctionSymbol::FunctionSymbol() : SymbolTableElement(std::make_shared<AttributePublic>()), arguments() , body(nullptr) {}


DataSize FunctionSymbol::GetSize()
{
    // Functions don't have a size
    return DataSize::Empty;
}