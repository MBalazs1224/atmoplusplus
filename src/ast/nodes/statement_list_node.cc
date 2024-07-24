#include "statement_list_node.hh"


void StatementListNode::Add(std::unique_ptr<StatementNode> node)
{
    statements.push_back(node);
}


StatementListNode::StatementListNode(std::unique_ptr<StatementNode> node)
{
    statements.push_back(node);
}