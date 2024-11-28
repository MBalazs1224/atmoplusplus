#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"

// left followed by right immediately
class IRSequence : IRStatement
{
        std::unique_ptr<IRExpression> left;
        std::unique_ptr<IRExpression> right;
    public:
       
        IRSequence(std::unique_ptr<IRExpression>,std::unique_ptr<IRExpression>);
};