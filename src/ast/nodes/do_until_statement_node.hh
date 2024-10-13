#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "body_node.hh"
#include <memory>
#include <iostream>

class DoUntilStatementNode : public Node
{
    std::unique_ptr<BodyNode> body;
    std::shared_ptr<IExpressionable> expression;

    public:
        DoUntilStatementNode(std::shared_ptr<IExpressionable> expression_in,std::unique_ptr<BodyNode> body_in, yy::location loc);

        ~DoUntilStatementNode() override;

        bool Check() override;
};