#pragma once
#include <memory>
#include "ir_expression.hh"
#include "../statements/ir_statement.hh"

// stmt is evaluated for side effects, then exp is evaluated for a result
class IREseq : IRExpression
{
        std::unique_ptr<IRExpression> exp;
        std::unique_ptr<IRStatement> stmt;
    public:
        IREseq(std::unique_ptr<IRExpression>, std::unique_ptr<IRStatement>);
};