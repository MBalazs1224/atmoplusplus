#pragma once
#include "statement_node.hh"
#include "body_node.hh"
#include <memory>
class ElseStatementNode : public  StatementNode
{
    std::unique_ptr<BodyNode> body;

    public:
        ElseStatementNode(std::unique_ptr<BodyNode> body_in) : body(std::move(body_in))
        {

        }
};