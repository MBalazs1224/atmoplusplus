#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"

// Evaluate the exoression and discard the result
class IREvaluateExpression : IRStatement
{
        std::shared_ptr<IRExpression> exp;
    public:
       
        IREvaluateExpression(std::shared_ptr<IRExpression>);
};