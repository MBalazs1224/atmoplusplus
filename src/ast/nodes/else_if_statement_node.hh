#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "body_node.hh"
#include <memory>
class ElseIfStatementNode : public  Node
{


    public:
        std::shared_ptr<IExpressionable> expression;
        std::unique_ptr<BodyNode> body;
        ElseIfStatementNode(std::shared_ptr<IExpressionable> expression_in ,std::unique_ptr<BodyNode> body_in, yy::location loc);
        ~ElseIfStatementNode() override;
        bool Check() override;
};