#include "function_definition_node.hh"

FunctionDefinitionNode::FunctionDefinitionNode(std::shared_ptr<FunctionSymbol> func_in)
    : function(std::move(func_in))
{
}

FunctionDefinitionNode::~FunctionDefinitionNode() = default;

bool FunctionDefinitionNode::Check()
{
    return function->Check();
}

std::shared_ptr<FunctionSymbol> FunctionDefinitionNode::GetFunction()
{
    return function;
}

std::shared_ptr<IRStatement> FunctionDefinitionNode::TranslateToIR()
{
    //TODO: Implement FunctionDefinition TranslateToIR

    return nullptr;
}
