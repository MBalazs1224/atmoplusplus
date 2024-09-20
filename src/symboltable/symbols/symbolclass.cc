#include "symbolclass.hh"

ClassSymbol::ClassSymbol(std::vector<std::shared_ptr<Identifier>> parents_in, std::unique_ptr<BodyNode> body_in)
    : parents(std::move(parents_in)), body(body_in->GetStatements()) {}

std::shared_ptr<Type> ClassSymbol::GetType() {
    return shared_from_this();
}

bool ClassSymbol::Equals(const std::shared_ptr<Type> other) {
    auto casted = std::dynamic_pointer_cast<ClassSymbol>(other);
    return casted && this->name == casted->name;
}

std::string ClassSymbol::ToString() {
    return Helper::FormatString("type class (%s)", name.c_str());
}

bool ClassSymbol::Check() {
    if (alreadyChecked) {
        return checkedResult;
    }
    alreadyChecked = true;
    for (auto &node : body) {
        if (auto variableDefinition = std::dynamic_pointer_cast<VariableDefinitionNode>(node)) {
            auto variable = variableDefinition->GetVariable();
            variables[variable->name] = variable;
        } else if (auto functionDefinition = std::dynamic_pointer_cast<FunctionDefinitionNode>(node)) {
            auto function = functionDefinition->GetFunction();
            functions[function->name] = function;
        } else {
            Error::ShowError("Only variable and/or function definitions can appear at the top level of a class!", node->location);
            checkedResult = false;
            return false;
        }
    }
    checkedResult = true;
    return true;
}

std::shared_ptr<FunctionSymbol> ClassSymbol::GetFunction(const std::string& id) {
    return functions[id];
}

std::shared_ptr<VariableSymbol> ClassSymbol::GetVariable(const std::string& id) {
    return variables[id];
}
