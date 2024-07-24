#pragma once
#include "body_node.hh"
#include "statement_node.hh"
#include "../expressions/expression.hh"

class UntilStatementNode : public StatementNode
{
    BodyNode& body;
    Expression& expression;

    public:
        UntilStatementNode(BodyNode& body, Expression& Expression) : body(body), expression(Expression)
        {
        }

        UntilStatementNode& operator=(const UntilStatementNode&);
};