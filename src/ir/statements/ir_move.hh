#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"

class IRMove : public IRStatement
{
    public:
        std::shared_ptr<IRExpression> source;
        std::shared_ptr<IRExpression> destination;
        

        IRMove(std::shared_ptr<IRExpression>, std::shared_ptr<IRExpression>);

        ~IRMove() override = default;

        std::string ToDotFormat(int&) override;
};
