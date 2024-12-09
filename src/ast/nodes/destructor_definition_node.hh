#pragma once
#include "node.hh"
#include "body_node.hh"
#include "../../symboltable/symbols/symbolfunction.hh"
class DestructorDefinitionNode : public Node
{
    private:
        // The function symbol of the destructor
        std::shared_ptr<FunctionSymbol> function;
    public:
    DestructorDefinitionNode(std::shared_ptr<FunctionSymbol> function_in,yy::location);

    // For default empty destructor in classes, will create an empty FunctionSymbol for the function
    DestructorDefinitionNode(yy::location location);


    ~DestructorDefinitionNode() override;
    bool Check() override;
    std::shared_ptr<FunctionSymbol> GetFunction();

    std::shared_ptr<IRStatement> TranslateToIR() override;
};