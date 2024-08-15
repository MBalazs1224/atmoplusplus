#pragma once
#include "statement_node.hh"
#include "../expressions/expressionable.hh"
#include <memory>
class ReturnStatementNode : public  StatementNode
{
    std::shared_ptr<IExpressionable> expression;
    public:
        ReturnStatementNode(std::shared_ptr<IExpressionable> exp_in) : expression(std::move(exp_in))
        {

        }
};