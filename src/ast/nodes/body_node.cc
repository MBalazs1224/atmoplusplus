#include "body_node.hh"

BodyNode::BodyNode(BodyNode&& other) : statementList(std::move(other.statementList))
{}

BodyNode::BodyNode(std::unique_ptr<StatementListNode> statements)
{
    this->statementList = std::move(statements);
}