#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "../../ir/reserved_ir_registers.hh"
#include <memory>
class ReturnStatementNode : public  Node
{
    std::shared_ptr<IExpressionable> expression;
    public:
        ReturnStatementNode(std::shared_ptr<IExpressionable> exp_in, yy::location loc);
        ~ReturnStatementNode() override;
        bool Check() override;
        
        std::shared_ptr<IRStatement> TranslateToIR() override;

};