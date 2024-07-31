#pragma once
#include "statement_node.hh"
#include "../expressions/expressionable.hh"
#include "else_statement_node.hh"
#include "body_node.hh"
#include <memory>
class IfStatementNode : public  StatementNode
{
    std::unique_ptr<IExpressionable> expression;
    std::unique_ptr<BodyNode> body;
    std::unique_ptr<ElseStatementNode> else_;
    public:
        IfStatementNode(std::unique_ptr<IExpressionable> expr_in,std::unique_ptr<BodyNode> body_in, std::unique_ptr<ElseStatementNode> else_in) : expression(std::move(expr_in)), body(std::move(body_in)), else_(std::move(else_in))
        {

        }
};