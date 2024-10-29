#include "destructor_definition_node.hh"

DestructorDefinitionNode::DestructorDefinitionNode(std::shared_ptr<FunctionSymbol> function_in, yy::location loc)
    : Node(loc), function(std::move(function_in))
{
}


DestructorDefinitionNode::DestructorDefinitionNode(yy::location location) : function(std::make_shared<FunctionSymbol>()) {
    this->location = location;
}


DestructorDefinitionNode::~DestructorDefinitionNode() = default;

std::shared_ptr<FunctionSymbol> DestructorDefinitionNode::GetFunction() {
    return function;
}

bool DestructorDefinitionNode::Check() {
    // TODO: Implement DestructorDefinitionNode::Check
    return true;
}