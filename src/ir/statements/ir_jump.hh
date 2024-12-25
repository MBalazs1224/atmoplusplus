#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"
#include "../../frame/labellist.hh"

// Transfer control to address exp
class IRJump : public IRStatement
{
        std::shared_ptr<IRExpression> exp;
        std::shared_ptr<LabelList> targets;
    public:
        
        IRJump(std::shared_ptr<IRExpression>, std::shared_ptr<LabelList>);
};