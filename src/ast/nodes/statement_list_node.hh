#pragma once
#include "node.hh"
#include "statement_node.hh"
#include <vector>
#include <memory>
class StatementListNode : public Node
{
    std::vector<std::unique_ptr<StatementNode>> statements;

    public:
        void Add(std::unique_ptr<StatementNode>);
        StatementListNode(std::unique_ptr<StatementNode> statement);
};