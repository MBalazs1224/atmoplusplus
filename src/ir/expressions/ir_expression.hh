#pragma once
#include <memory>
#include "../ir_node.hh"

class IRExpressionList;

class IRExpression : public IRNode
{
    public:
        virtual ~IRExpression() = 0;
        virtual std::shared_ptr<IRExpression> Build (std::shared_ptr<IRExpressionList> kids) = 0;
        virtual std::shared_ptr<IRExpressionList> Kids () = 0;
};