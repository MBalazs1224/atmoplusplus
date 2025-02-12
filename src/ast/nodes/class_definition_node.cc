#include "class_definition_node.hh"

// Constructor: initializes the class symbol
ClassDefinitionNode::ClassDefinitionNode(std::shared_ptr<ClassSymbol> func_in)
    : class_(std::move(func_in))
{
}

// Destructor: the default implementation is used as there is no specific cleanup required
ClassDefinitionNode::~ClassDefinitionNode() = default;

// Check function: delegates the check operation to the class symbol
bool ClassDefinitionNode::Check()
{
    return class_->Check();
}

// GetClass function: returns the stored class symbol
std::shared_ptr<ClassSymbol> ClassDefinitionNode::GetClass()
{
    return class_;
}

std::shared_ptr<IRStatement> ClassDefinitionNode::TranslateToIR()
{
    return class_->TranslateToIR();
}
