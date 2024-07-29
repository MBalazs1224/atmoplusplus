#pragma once
#include "body_node.hh"
#include "statement_node.hh"
#include "../expressions/expression.hh"
#include <memory>
#include <iostream>

class DoUntilStatementNode
{
    std::unique_ptr<BodyNode> body;
    std::unique_ptr<Expression> expression;

    public:
        DoUntilStatementNode(std::unique_ptr<Expression> expression_in,std::unique_ptr<BodyNode> body_in)
        : body(std::move(body_in)), expression(std::move(expression_in))
        {
        }
};