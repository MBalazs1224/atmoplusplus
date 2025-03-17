#pragma once
#include <memory>
#include <stdexcept>
#include "ir_statement.hh"
#include "../expressions/relational_operators.hh"
#include "../expressions/ir_expression.hh"
#include "ir_label.hh"

// Evaluate left and right in that order and compare their results using relop, if the result is true jump to iftrue, otherwise iffalse
class IRCJump : public IRStatement
{
    public:
        RelationalOperator relop;
        std::shared_ptr<IRExpression> left;
        std::shared_ptr<IRExpression> right;

        std::shared_ptr<Label> iftrue;
        std::shared_ptr<Label> iffalse;

        RelationalOperator OppositeOperator(RelationalOperator);
       

        IRCJump(RelationalOperator,std::shared_ptr<IRExpression>,std::shared_ptr<IRExpression>,std::shared_ptr<Label>,std::shared_ptr<Label>);

        ~IRCJump() override = default;

        std::string ToDotFormat(int&) override;

        std::string OperatorToString();
};