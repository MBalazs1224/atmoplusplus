#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"

class IRMove : IRStatement
{
        std::unique_ptr<IRExpression> source;
        std::unique_ptr<IRExpression> destination;
    public:
        

        IRMove(std::unique_ptr<IRExpression>, std::unique_ptr<IRExpression>);
};