#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"

// Evaluate the exoression and discard the result
class IREvaluateExpression : public IRStatement
{
    public:
        std::shared_ptr<IRExpression> exp;
       
        IREvaluateExpression(std::shared_ptr<IRExpression>);

        std::string ToDotFormat(int&) override;
        
        std::shared_ptr<IRStatement> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids () override;
};