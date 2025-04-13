#pragma once
#include "../statements/ir_statement.hh"
#include "../statements/ir_move.hh"
#include "../expressions/ir_call.hh"
#include "../expressions/ir_temp.hh"
class IRMoveCall : public IRStatement
{
    public:
        std::shared_ptr<IRTemp> destination;
        std::shared_ptr<IRCall> source;

        std::shared_ptr<IRExpressionList> Kids() override;
        std::shared_ptr<IRStatement> Build(std::shared_ptr<IRExpressionList>) override;

        IRMoveCall(std::shared_ptr<IRTemp>, std::shared_ptr<IRCall>);
        
        std::string ToDotFormat(int&) override;

};