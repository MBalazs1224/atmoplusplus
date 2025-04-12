#pragma once
#include <memory>
#include "../ir_node.hh"
#include "../expressions/ir_expression_list.hh"
class IRStatement : public IRNode
{
    public:
        virtual std::shared_ptr<IRStatement> Build (std::shared_ptr<IRExpressionList> kids) = 0;
        virtual std::shared_ptr<IRExpressionList> Kids () = 0;
        virtual ~IRStatement() = 0;
};