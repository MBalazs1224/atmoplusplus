#include "body_node.hh"
#include "function_definition_node.hh"

BodyNode::BodyNode(BodyNode &&other)
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
    for (auto &statement : statements)
    {
        if(!StatementIsValid(statement))
        {
            Error::ShowError("Invalid statement!",statement->location);
            continue;
        }
        // Functions can only be created on the root node, meaning it cannot be inside a body (classes receive the statements inside them through a vector not a body )

        statement->Check();
    }
    return true;
}

bool BodyNode::StatementIsValid(const std::shared_ptr<Node> statement)
{
    if (auto function_definition = std::dynamic_pointer_cast<FunctionDefinitionNode>(statement))
    {
        Error::ShowError("Functions can only be created on the root level!", statement->location);
        return false;
    }

    if (auto class_definition = std::dynamic_pointer_cast<ClassDefinitionNode>(statement))
    {
        Error::ShowError("Functions can only be created on the root level!", statement->location);
        return true;
    }

    if(auto expression = std::dynamic_pointer_cast<IExpressionable>(statement))
    {
        // Only assignment expressions and function calls can be used as a statement 

        return std::dynamic_pointer_cast<AssignmentExpression>(expression) != nullptr || std::dynamic_pointer_cast<FunctionCall>(expression) != nullptr;
    }

    // The only node not legal as a statement is constructor definition

    return std::dynamic_pointer_cast<ConstructorDefinitionNode>(statement) == nullptr;
}

bool BodyNode::isEmpty()
{
    return statements.empty();
}

std::vector<std::shared_ptr<Node>> BodyNode::GetStatements()
{
    return statements;
}