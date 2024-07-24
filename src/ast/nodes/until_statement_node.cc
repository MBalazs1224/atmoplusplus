#include "until_statement_node.hh"

UntilStatementNode& UntilStatementNode::operator=(const UntilStatementNode& other)
{
    other.body = this->body;
    other.expression = this->expression;
}