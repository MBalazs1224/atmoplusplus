#pragma once
#include <memory>
#include "ir_expression.hh"
#include "ir_expression_list.hh"
#include "../../frame/label.hh"


// Represents a function call, func is evaluated before args who are evaluated left to right
class IRCall : public IRExpression
{
    public:
        std::shared_ptr<IRExpression> func;
        std::shared_ptr<IRExpressionList> args;

        // Indicates, wether the return value is needed later (in case of void functions and constructors it's not)
        bool returnValueNeeded = true;

        std::shared_ptr<IRExpressionList> argumentLocations;

        IRCall(std::shared_ptr<IRExpression>, std::shared_ptr<IRExpressionList>, std::shared_ptr<IRExpressionList>, bool);

        std::string ToDotFormat(int&) override;
};