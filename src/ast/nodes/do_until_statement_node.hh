#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "body_node.hh"
#include <memory>
#include <iostream>

class DoUntilStatementNode : public Node
{

    public:
        std::unique_ptr<BodyNode> body;
        std::shared_ptr<IExpressionable> expression;

        DoUntilStatementNode(std::shared_ptr<IExpressionable> expression_in,std::unique_ptr<BodyNode> body_in, yy::location loc);

        ~DoUntilStatementNode() override;

        bool Check() override;
};