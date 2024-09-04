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
    //TODO: Implement body node checking
}

bool BodyNode::isEmpty()
{
    return statements.empty();
}