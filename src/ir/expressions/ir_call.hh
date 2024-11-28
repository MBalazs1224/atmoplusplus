#pragma once
#include <memory>
#include "ir_expression.hh"
#include "ir_expression_list.hh"
#include "../../frame/label.hh"


// Represents a function call, func is evaluated before args who are evaluated left to right
class IRCall : IRExpression
{
        std::unique_ptr<IRExpression> func;
        std::unique_ptr<ExpressionList> args;
    public:
        IRCall(std::unique_ptr<IRExpression>, std::unique_ptr<ExpressionList>);
};