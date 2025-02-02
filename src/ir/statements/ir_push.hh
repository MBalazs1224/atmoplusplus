#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"

class IRPush : public IRStatement
{
    std::shared_ptr<IRExpression> exp;

    public:
        IRPush(std::shared_ptr<IRExpression>);

        std::string ToDotFormat(int&) override;
};