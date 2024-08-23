#pragma once
#include "body_node.hh"
#include "node.hh"
#include "../expressions/expressionable.hh"
#include <memory>
#include <iostream>

class UntilStatementNode : public Node
{
    std::unique_ptr<BodyNode> body;
    std::shared_ptr<IExpressionable> expression;

    public:
        UntilStatementNode(std::shared_ptr<IExpressionable> expression_in,std::unique_ptr<BodyNode> body_in,yy::location loc)
        : body(std::move(body_in)), expression(std::move(expression_in)), Node(loc)
        {
        }
        ~UntilStatementNode() override = default;
};