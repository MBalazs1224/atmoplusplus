#pragma once
#include <memory>
#include "ir_statement.hh"
#include "relational_operators.hh"
#include "../expressions/ir_expression.hh"
#include "ir_label.hh"

// Evaluate left and right in that order and compare their results using relop, if the result is true jump to iftrue, otherwise iffalse
class IRCJump : IRStatement
{
        RelationalOperator relop;
        std::unique_ptr<IRExpression> left;
        std::unique_ptr<IRExpression> right;

        IRLabel iftrue;
        IRLabel iffalse;
    public:
       

        IRCJump(std::unique_ptr<IRExpression>,std::unique_ptr<IRExpression>,IRLabel,IRLabel);
};