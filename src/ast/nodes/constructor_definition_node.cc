#include "constructor_definition_node.hh"

// Constructor: initializes the function symbol
ConstructorDefinitionNode::ConstructorDefinitionNode(std::shared_ptr<FunctionSymbol> function_in, std::vector<std::shared_ptr<IExpressionable>> args_in)
    : function(std::move(function_in)), arguments_to_parent(args_in)
{
}

// Destructor: uses default since no custom cleanup is required
ConstructorDefinitionNode::~ConstructorDefinitionNode() = default;

// Check function: delegates the check operation to the function symbol
bool ConstructorDefinitionNode::Check()
{
    return function->Check();
}

// GetFunction function: returns the stored function symbol
std::shared_ptr<FunctionSymbol> ConstructorDefinitionNode::GetFunction()
{
    return function;
}
