#pragma once
#include "node.hh"
#include "../../symboltable/symbols/symbolfunction.hh"
#include "../attributes/attribute.hh"
#include "../types/type.hh"
#include <memory>
#include <vector>

class FunctionDefinitionNode : public Node
{
    // Pointer to the created funtion
    std::shared_ptr<FunctionSymbol> function;
    
    public:
        FunctionDefinitionNode( std::shared_ptr<FunctionSymbol> func_in);
        ~FunctionDefinitionNode() override;
        bool Check() override;

    std::shared_ptr<FunctionSymbol> GetFunction();
};