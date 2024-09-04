#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "else_statement_node.hh"
#include "else_if_statement_node.hh"
#include "body_node.hh"
#include <memory>
class IfStatementNode : public  Node
{
    std::shared_ptr<IExpressionable> expression;
    std::unique_ptr<BodyNode> body;
    std::vector<std::unique_ptr<ElseIfStatementNode>> else_ifs;
    std::unique_ptr<ElseStatementNode> else_;
    public:
        IfStatementNode(std::shared_ptr<IExpressionable> expr_in,std::unique_ptr<BodyNode> body_in,std::vector<std::unique_ptr<ElseIfStatementNode>> else_ifs_in, std::unique_ptr<ElseStatementNode> else_in, yy::location loc) :Node(loc),  expression(std::move(expr_in)), body(std::move(body_in)),  else_ifs(std::move(else_ifs_in)), else_(std::move(else_in))
        {

        }
        ~IfStatementNode() override = default;
        void Check() override
        {
            //TODO: Implement if node checking
        }
};