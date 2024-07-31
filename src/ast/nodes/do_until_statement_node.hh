#pragma once
#include "body_node.hh"
#include "statement_node.hh"
#include "../expressions/expressionable.hh"
#include <memory>
#include <iostream>

class DoUntilStatementNode
{
    std::unique_ptr<BodyNode> body;
    std::unique_ptr<IExpressionable> expression;

    public:
        DoUntilStatementNode(std::unique_ptr<IExpressionable> expression_in,std::unique_ptr<BodyNode> body_in)
        : body(std::move(body_in)), expression(std::move(expression_in))
        {
        }
};