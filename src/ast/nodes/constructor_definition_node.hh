#pragma once
#include "node.hh"
#include "body_node.hh"
#include "../../symboltable/symbols/symbolfunction.hh"
class ConstructorDefinitionNode : public Node
{
    private:
        // The function symbol of the constructor
        std::shared_ptr<FunctionSymbol> function;

        // The arguments to parent for correct chaining
        std::vector<std::shared_ptr<IExpressionable>> arguments_to_parent;
    public:
    ConstructorDefinitionNode(std::shared_ptr<FunctionSymbol> function_in, std::vector<std::shared_ptr<IExpressionable>>, yy::location);

    // For default empty constructors in classes, will create an empty FunctionSymbol for the function
    ConstructorDefinitionNode(yy::location location);


    ~ConstructorDefinitionNode() override;
    bool Check() override;
    const std::vector<std::shared_ptr<IExpressionable>>& GetArgumentsForParentConstructor();
    std::shared_ptr<FunctionSymbol> GetFunction();
};