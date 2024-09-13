#include "body_node.hh"

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
    for (auto &statement : statements)
    {
        //Might need to stop checking if one of the statements is wrong
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