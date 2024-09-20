#pragma once
#include "../symboltableelement.hh"
#include "symbolvariable.hh"
#include "symbolfunction.hh"
#include "../../ast/nodes/body_node.hh"
#include <memory>
#include <vector>
#include "../../ast/types/type.hh"
#include "../../ast/nodes/variable_definition_node.hh"
#include "../../ast/nodes/function_definition_node.hh"
#include "../../ast/expressions/identifier.hh"
#include <unordered_map>

class ClassSymbol : public SymbolTableElement, public Type, /*std::enable_shared_from_this makes it possible to retrieve a shared_ptr from this that the GetType can return*/ public std::enable_shared_from_this<ClassSymbol>
{
    private:
        // To prevent multiple checking when accessing multiple things inside the same class type
        bool alreadyChecked = false;

        //The result of the first check so we can return it again if the class symbol is used later
        bool checkedResult;

        std::vector<std::shared_ptr<Identifier>> parents;
        std::vector<std::shared_ptr<Node>> body;

        // The hashmap storing the variables inside the class
        std::unordered_map<std::string, std::shared_ptr<VariableSymbol>> variables;

        // The hashmap storing the variables inside the class
        std::unordered_map<std::string, std::shared_ptr<FunctionSymbol>> functions;
    public:
    ClassSymbol(std::vector<std::shared_ptr<Identifier>> parents_in, std::unique_ptr<BodyNode> body_in);

    std::shared_ptr<Type> GetType() override;

    bool Equals(const std::shared_ptr<Type> other) override;

    std::string ToString() override;

    bool Check() override;
    // Will return the function based on the given ID or null if it wasn't found
    std::shared_ptr<FunctionSymbol> GetFunction(const std::string& id);

    // Will return the variable based on the given ID or null if it wasn't found
    std::shared_ptr<VariableSymbol> GetVariable(const std::string&);
};