#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"

class IRPush : public IRStatement, public std::enable_shared_from_this<IRPush>
{
    public:
    std::shared_ptr<IRExpression> exp;

        IRPush(std::shared_ptr<IRExpression>);

        std::string ToDotFormat(int&) override;

                                                        
        std::shared_ptr<IRStatement> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids () override;
};