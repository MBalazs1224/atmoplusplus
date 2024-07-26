#pragma once
#include "node.hh"
#include "statement_node.hh"
#include "statement_list_node.hh"
#include <vector>
#include <memory>
class BodyNode : public Node
{
    std::unique_ptr<StatementListNode> statementList;

    public:
    BodyNode(std::unique_ptr<StatementListNode>);

    BodyNode(BodyNode&& other);
};