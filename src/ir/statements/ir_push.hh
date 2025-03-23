#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"

class IRPush : public IRStatement
{
    public:
    std::shared_ptr<IRExpression> exp;

        IRPush(std::shared_ptr<IRExpression>);

        std::string ToDotFormat(int&) override;
};