#pragma once
#include <memory>
#include "ir_statement.hh"
#include "relational_operators.hh"
#include "../expressions/ir_expression.hh"
#include "ir_label.hh"

// Evaluate left and right in that order and compare their results using relop, if the result is true jump to iftrue, otherwise iffalse
class IRCJump : public IRStatement
{
        RelationalOperator relop;
        std::shared_ptr<IRExpression> left;
        std::shared_ptr<IRExpression> right;

        IRLabel iftrue;
        IRLabel iffalse;
    public:
       

        IRCJump(std::shared_ptr<IRExpression>,std::shared_ptr<IRExpression>,IRLabel,IRLabel);

        ~IRCJump() override = default;
};