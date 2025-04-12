#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../expressions/ir_expression.hh"
#include "../expressions/ir_mem.hh"
#include "../../frame/datasize.hh"

class IRMove : public IRStatement
{
    public:
        std::shared_ptr<IRExpression> source;
        std::shared_ptr<IRExpression> destination;
        

        IRMove(std::shared_ptr<IRExpression>, std::shared_ptr<IRExpression>);

        ~IRMove() override = default;

        std::string ToDotFormat(int&) override;

                                        
        std::shared_ptr<IRStatement> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids () override;
};
