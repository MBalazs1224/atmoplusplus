#pragma once
#include "node.hh"
#include <vector>
#include <memory>
class StatementListNode : public Node
{
    std::vector<std::shared_ptr<Node>> statements;

    public:
        void Add(std::shared_ptr<Node>);
        StatementListNode(std::shared_ptr<Node> statement);
        std::vector<std::shared_ptr<Node>> GetStatements();
};