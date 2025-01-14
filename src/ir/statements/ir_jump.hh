#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"
#include "../../frame/labellist.hh"

// Transfer control to address exp, targets are later sued for dataflow analysis
class IRJump : public IRStatement
{
        std::shared_ptr<IRExpression> exp;
        std::shared_ptr<LabelList> targets;
    public:
        
        IRJump(std::shared_ptr<IRExpression>, std::shared_ptr<LabelList>);
        IRJump(std::shared_ptr<IRExpression>);

        ~IRJump() override = default;
};