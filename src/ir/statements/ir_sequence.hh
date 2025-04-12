#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"
#include <vector>
#include <assert.h>
#include <stdexcept>
// left followed by right immediately
class IRSequence : public IRStatement
{
    public:
        std::shared_ptr<IRStatement> left;
        std::shared_ptr<IRStatement> right;
       
        IRSequence(std::shared_ptr<IRStatement>,std::shared_ptr<IRStatement>);

        // Will convert the vector elements into a IRSequence tree
        IRSequence(const std::vector<std::shared_ptr<IRStatement>>&);

        ~IRSequence() override = default;    

        std::string ToDotFormat(int&) override;

                                                                        
        std::shared_ptr<IRStatement> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids () override;
};