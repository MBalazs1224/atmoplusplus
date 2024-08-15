#pragma once
#include "node.hh"
#include "statement_node.hh"
#include "statement_list_node.hh"
#include <vector>
#include <memory>
class BodyNode : public Node
{
    std::vector<std::unique_ptr<StatementNode>> statements;
    public:
    BodyNode(std::vector<std::unique_ptr<StatementNode>>);

    BodyNode(BodyNode&& other);
};