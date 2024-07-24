#pragma once
#include "node.hh"
#include "statement_node.hh"
#include <vector>
class BodyNode : public Node
{
    std::vector<StatementNode> statements;

    public:
    BodyNode();
};