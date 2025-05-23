#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"
#include "../expressions/ir_name.hh"
#include "../../frame/labellist.hh"

// Transfer control to address exp, targets are later sued for dataflow analysis
class IRJump : public IRStatement
{
    public:
        std::shared_ptr<IRExpression> exp;
        std::shared_ptr<LabelList> targets;
        
        IRJump(std::shared_ptr<IRExpression>, std::shared_ptr<LabelList>);
        IRJump(std::shared_ptr<IRName>);

        ~IRJump() override = default;

        std::string ToDotFormat(int&) override;
                
        std::shared_ptr<IRStatement> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids () override;
};