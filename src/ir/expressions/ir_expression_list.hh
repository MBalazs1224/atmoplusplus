#pragma once
#include <memory>
#include <vector>
#include "../ir_node.hh"
#include "ir_expression.hh"

class IRExpressionList : public IRNode
{

    public:
        std::shared_ptr<IRExpression> expression;
        std::shared_ptr<IRExpressionList> next;

        std::string ToDotFormat(int&) override;

        // Will turn the vector of expressions into an expresison list
        IRExpressionList(std::vector<std::shared_ptr<IRExpression>>&);

        // We still need an empty constructor
        
        IRExpressionList() = default;
};