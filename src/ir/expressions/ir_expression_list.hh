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

        /*
            Will turn the given vector into a linked list.
         */
        static std::shared_ptr<IRExpressionList> CreateFromVector(std::vector<std::shared_ptr<IRExpression>>&);

        // We still need an empty constructor
        
        IRExpressionList() = default;
};