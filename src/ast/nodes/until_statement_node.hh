#pragma once
#include "body_node.hh"
#include "statement_node.hh"
#include "../expressions/expression.hh"
#include <memory>

class UntilStatementNode : public StatementNode
{
    std::unique_ptr<BodyNode> body;
    std::unique_ptr<Expression> expression;

    public:
        UntilStatementNode(std::unique_ptr<BodyNode> body, std::unique_ptr<Expression> expression)
        : body(std::move(body)), expression(std::move(expression))
        {
        }
};