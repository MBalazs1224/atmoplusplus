#pragma once
#include <memory>
#include "ir_expression.hh"
#include "../statements/ir_statement.hh"
#include <stdexcept>
// stmt is evaluated for side effects, then exp is evaluated for a result
class IREseq : public IRExpression
{
    public:
        std::shared_ptr<IRExpression> exp;
        std::shared_ptr<IRStatement> stmt;
        IREseq(std::shared_ptr<IRExpression>, std::shared_ptr<IRStatement>);

        std::string ToDotFormat(int&) override;

        std::shared_ptr<IRExpression> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids() override;
};