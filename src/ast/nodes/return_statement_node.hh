#pragma once
#include "statement_node.hh"
#include "../expressions/expressionable.hh"
#include <memory>
class ReturnStatementNode : public  StatementNode
{
    std::unique_ptr<IExpressionable> expression;
    public:
        ReturnStatementNode(std::unique_ptr<IExpressionable> exp_in) : expression(std::move(exp_in))
        {

        }
};