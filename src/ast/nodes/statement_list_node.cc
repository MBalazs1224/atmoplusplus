#include "statement_list_node.hh"


void StatementListNode::Add(std::shared_ptr<Node> node)
{
    statements.push_back(std::move(node));
}


StatementListNode::StatementListNode(std::shared_ptr<Node> node)
{
    if (node)
    {
        statements.push_back(std::move(node));
    }
    

}

std::vector<std::shared_ptr<Node>> StatementListNode::GetStatementsRef()
{
    return statements;
} 

std::vector<std::shared_ptr<Node>> StatementListNode::GetStatements()
{
    return std::move(statements);
}
bool StatementListNode::Check()
{
    //TODO: Implement statementlist node checking
    return true;
}

std::shared_ptr<IRStatement> StatementListNode::TranslateToIR()
{
    //TODO: Implement StatementListNode TranslateToIR

    return nullptr;
}