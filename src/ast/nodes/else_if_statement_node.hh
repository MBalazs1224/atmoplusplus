#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "body_node.hh"
#include <memory>
class ElseIfStatementNode : public  Node
{
    std::shared_ptr<IExpressionable> expression;
    std::unique_ptr<BodyNode> body;

    public:
        ElseIfStatementNode(std::shared_ptr<IExpressionable> expression_in ,std::unique_ptr<BodyNode> body_in, yy::location loc) : Node(loc), expression(std::move(expression_in)), body(std::move(body_in))
        {

        }
        ~ElseIfStatementNode() override = default;
};