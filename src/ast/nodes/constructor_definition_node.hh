#pragma once
#include "node.hh"
#include "body_node.hh"
#include "../../symboltable/symbols/symbolfunction.hh"
class ConstructorDefinitionNode : public Node
{
    private:
        // The function symbol of the constructor
        std::shared_ptr<FunctionSymbol> function;
    public:
    ConstructorDefinitionNode(std::shared_ptr<FunctionSymbol> function_in);

    ~ConstructorDefinitionNode() override;
    bool Check() override;

    std::shared_ptr<FunctionSymbol> GetFunction();
};