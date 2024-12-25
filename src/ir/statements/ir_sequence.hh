#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"
#include <vector>
#include <assert.h>
// left followed by right immediately
class IRSequence : public IRStatement
{
        std::shared_ptr<IRStatement> left;
        std::shared_ptr<IRStatement> right;
    public:
       
        IRSequence(std::shared_ptr<IRStatement>,std::shared_ptr<IRStatement>);

        // Will convert the vector elements into a IRSequence tree
        IRSequence(const std::vector<std::shared_ptr<IRStatement>>&);
};