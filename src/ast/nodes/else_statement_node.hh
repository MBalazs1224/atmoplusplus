#pragma once
#include "node.hh"
#include "body_node.hh"
#include <memory>
class ElseStatementNode : public  Node
{
    std::unique_ptr<BodyNode> body;

    public:
        ElseStatementNode(std::unique_ptr<BodyNode> body_in, yy::location loc);
        ~ElseStatementNode() override;
        bool Check() override;
};