#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include <memory>
class ReturnStatementNode : public  Node
{
    std::shared_ptr<IExpressionable> expression;
    public:
        ReturnStatementNode(std::shared_ptr<IExpressionable> exp_in, yy::location loc) :Node(loc),  expression(std::move(exp_in)) 
        {

        }
        ~ReturnStatementNode() override = default;
        void Check() override
        {
            //TODO: Implement return node checking
        }
};