#pragma once
#include "node.hh"
#include "body_node.hh"
class ConstructorDefinitionNode : public Node
{
    private:
        // The function symbol of the constructor
        std::shared_ptr<FunctionSymbol> function;
    public:
    ConstructorDefinitionNode(std::shared_ptr<FunctionSymbol> function_in) : function(function_in)
    {
    }

    ~ConstructorDefinitionNode() override = default;
    bool Check() override
    {
        return function->Check();
    }

    std::shared_ptr<FunctionSymbol> GetFunction()
    {
        return function;
    }
};