#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"

class IRPop : public IRStatement, public std::enable_shared_from_this<IRPop>
{
    public:
    std::shared_ptr<IRExpression> exp;

        IRPop(std::shared_ptr<IRExpression>);
        std::string ToDotFormat(int&) override;

                                                
        std::shared_ptr<IRStatement> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids () override;
};