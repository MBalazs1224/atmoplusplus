#pragma once
#include <memory>
#include "ir_expression.hh"
#include "ir_expression_list.hh"
#include "../../frame/label.hh"


// Represents a function call, func is evaluated before args who are evaluated left to right
class IRCall : IRExpression
{
        std::shared_ptr<IRExpression> func;
        std::shared_ptr<ExpressionList> args;
    public:
        IRCall(std::shared_ptr<IRExpression>, std::shared_ptr<ExpressionList>);
};