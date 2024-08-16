#include "statement_list_node.hh"


void StatementListNode::Add(std::unique_ptr<StatementNode> node)
{
    statements.push_back(std::move(node));
}


StatementListNode::StatementListNode(std::unique_ptr<StatementNode> node)
{
    if (node)
    {
        statements.push_back(std::move(node));
    }
    

}

std::vector<std::unique_ptr<StatementNode>> StatementListNode::GetStatements()
{
    return std::move(statements);
}