#pragma once
#include "../statements/ir_statement.hh"
#include "../statements/ir_evaluate_expression.hh"
#include "../expressions/ir_call.hh"
#include "../expressions/ir_temp.hh"
class IRExpCall : public IRStatement
{
    public:
        std::shared_ptr<IRCall> call;

        std::shared_ptr<IRExpressionList> Kids() override;
        std::shared_ptr<IRStatement> Build(std::shared_ptr<IRExpressionList>) override;

        IRExpCall(std::shared_ptr<IRCall>);

        std::string ToDotFormat(int&) override;

};