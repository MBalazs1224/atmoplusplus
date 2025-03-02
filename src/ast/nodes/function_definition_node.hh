#pragma once
#include "node.hh"
#include "../../symboltable/symbols/symbolfunction.hh"
#include "../attributes/attribute.hh"
#include "../types/type.hh"
#include "../../frame/x86_frame.hh"
#include <memory>
#include <vector>

class FunctionDefinitionNode : public Node
{
    // Pointer to the created funtion
    std::shared_ptr<FunctionSymbol> function;

    // Wil return a list that will indicate wether the local variables escape the frame
    std::shared_ptr<BoolList> GetWetherVariablesEscape();

    
    public:
        FunctionDefinitionNode( std::shared_ptr<FunctionSymbol> func_in);
        ~FunctionDefinitionNode() override;
        bool Check() override;

    std::shared_ptr<FunctionSymbol> GetFunction();

    std::shared_ptr<IRStatement> TranslateToIR() override;
};