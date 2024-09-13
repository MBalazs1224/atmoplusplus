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
        // Will move the vector, making it invalid afterwards
        std::vector<std::shared_ptr<Node>> GetStatements();
        ~StatementListNode() override = default;
        // Will not move the vector
        std::vector<std::shared_ptr<Node>> GetStatementsRef();
        bool Check() override;
};