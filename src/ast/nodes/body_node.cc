#include "body_node.hh"

BodyNode::BodyNode(BodyNode&& other)
{
    this->statements = std::move(other.statements);
}

BodyNode::BodyNode(std::vector<std::shared_ptr<Node>> statements)
{
    this->statements = std::move(statements);
}

void BodyNode::Check()
{
    // TODO: Might need to calculate the size of teh body here
    for (auto &statement : statements)
    {
        statement->Check();
    }
}

bool BodyNode::isEmpty()
{
    return statements.empty();
}

std::vector<std::shared_ptr<Node>> BodyNode::GetStatements()
{
    return statements;
}