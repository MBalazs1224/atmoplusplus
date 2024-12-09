#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"

// left followed by right immediately
class IRSequence : IRStatement
{
        std::shared_ptr<IRExpression> left;
        std::shared_ptr<IRExpression> right;
    public:
       
        IRSequence(std::shared_ptr<IRExpression>,std::shared_ptr<IRExpression>);
};