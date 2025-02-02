#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"

class IRPop : public IRStatement
{
    std::shared_ptr<IRExpression> exp;

    public:
        IRPop(std::shared_ptr<IRExpression>);
        std::string ToDotFormat(int&) override;
};