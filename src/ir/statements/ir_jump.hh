#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"
#include "../../frame/labellist.hh"

// Transfer control to address exp
class IRJump : IRStatement
{
        std::unique_ptr<IRExpression> exp;
        std::unique_ptr<LabelList> targets;
    public:
        
        IRJump(std::unique_ptr<IRExpression>, std::unique_ptr<LabelList>);
};