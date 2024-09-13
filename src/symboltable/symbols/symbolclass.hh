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


class ClassSymbol : public SymbolTableElement, public Type, /*std::enable_shared_from_this makes it possible to retrieve a shared_ptr from this that the GetType can return*/ public std::enable_shared_from_this<ClassSymbol>
{
    private:
        // To prevent multiple checking when accessing multiple things inside the same class type
        bool alreadyChecked = false;

        //The result of the first check so we can return it again if the class symbol is used later
        bool checkedResult;

        std::vector<std::shared_ptr<Identifier>> parents;
        std::unique_ptr<BodyNode> body;

        // The hashmap storing the variables inside the class
        std::unordered_map<std::string, std::shared_ptr<VariableSymbol>> variables;

        // The hashmap storing the variables inside the class
        std::unordered_map<std::string, std::shared_ptr<FunctionSymbol>> functions;
    public:
    ClassSymbol(std::vector<std::shared_ptr<Identifier>> parents_in, std::unique_ptr<BodyNode> body_in) : parents(std::move(parents_in)), body(std::move(body_in))
    {
        
    }

    std::shared_ptr<Type> GetType() override
    {
        return shared_from_this();
    }

    bool Equals(const std::shared_ptr<Type> other) override
    {
        auto casted = std::dynamic_pointer_cast<ClassSymbol>(other);
        return casted && this->name == casted->name;
    }

    std::string ToString() override
    {
        // FIXME: FormatString should be in a helper class
        return Error::FormatString("type class (%s)",name.c_str());
    }

    bool Check() override
    {
        if (alreadyChecked)
        {
            return checkedResult;
        }
        alreadyChecked = true;
        for (auto &node : body->GetStatements())
        {
            // If it's a variable definition insert it into the variable hashmap for later access
            if (auto variableDefinition = std::dynamic_pointer_cast<VariableDefinitionNode>(node))
            {
                auto variable = variableDefinition->GetVariable();
                variables[variable->name] = variable;
            }
            else if (auto functionDefinition = std::dynamic_pointer_cast<FunctionDefinitionNode>(node))
            {
                auto function = functionDefinition->GetFunction();
                functions[function->name] = function;
            }
            else
            {
                Error::ShowError("Only variable and/or function definitions can appear at the top level of a class!",node->location);
                checkedResult = false;
                return false;
            }
            
        }
        checkedResult = true;
        return true;
    }
    // Will return the function based on the given ID or null if it wasn't found
    std::shared_ptr<FunctionSymbol> GetFunction(const std::string& id)
    {
        return functions[id];
    }

    // Will return the variable based on the given ID or null if it wasn't found
    std::shared_ptr<VariableSymbol> GetVariable(const std::string& id)
    {
        return variables[id];
    }
};