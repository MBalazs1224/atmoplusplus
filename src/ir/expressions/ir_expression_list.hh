#pragma once
#include <memory>
#include "../ir_node.hh"
#include "ir_expression.hh"

class IRExpressionList : public IRNode
{

    public:
        std::shared_ptr<IRExpression> expression;
        std::shared_ptr<IRExpressionList> next;

        std::string ToDotFormat(int&) override;
};