#include "body_node.hh"

BodyNode::BodyNode(BodyNode&& other)
{
    this->statements = std::move(other.statements);
}

BodyNode::BodyNode(std::vector<std::unique_ptr<StatementNode>> statements)
{
    this->statements = std::move(statements);
}