#pragma once
#include <memory>
#include "ir_expression.hh"
#include "ir_expression_list.hh"
#include "../../frame/label.hh"


// Represents a function call, func is evaluated before args who are evaluated left to right
class IRCall : public IRExpression
{
        std::shared_ptr<IRExpression> func;
        std::shared_ptr<IRExpressionList> args;
    public:
        IRCall(std::shared_ptr<IRExpression>, std::shared_ptr<IRExpressionList>);

        std::string ToDotFormat(int&) override;
};