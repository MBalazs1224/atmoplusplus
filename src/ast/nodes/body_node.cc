#include "body_node.hh"
#include "function_definition_node.hh"

BodyNode::BodyNode(BodyNode&& other)
{
    this->statements = std::move(other.statements);
}

BodyNode::BodyNode(std::vector<std::shared_ptr<Node>> statements)
{
    this->statements = std::move(statements);
}

bool BodyNode::Check()
{
    // TODO: Might need to calculate the size of teh body here
    for (auto& statement : statements)
    {
        // Functions can only be created on the root node, meaning it cannot be inside a body (classes receive the statements inside them through a vector not a body )
        auto function_definition = std::dynamic_pointer_cast<FunctionDefinitionNode>(statement);
        if (function_definition)
        {
            Error::ShowError("Functions can only be created on the root level!",function_definition->location);

            // Check the rest of the statements, so we can show all error messages
            continue;
        }
        statement->Check();
    }
    return true;
}

bool BodyNode::isEmpty()
{
    return statements.empty();
}

std::vector<std::shared_ptr<Node>> BodyNode::GetStatements()
{
    return statements;
}