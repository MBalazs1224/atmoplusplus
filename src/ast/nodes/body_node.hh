#pragma once
#include "node.hh"
#include "statement_list_node.hh"
#include <vector>
#include <memory>
class BodyNode : public Node
{
    std::vector<std::shared_ptr<Node>> statements;
    public:
    BodyNode(std::vector<std::shared_ptr<Node>>);
    BodyNode() = default;

    BodyNode(BodyNode&& other);
    public:
        ~BodyNode() override = default;

        void Check() override;
        bool isEmpty();
};