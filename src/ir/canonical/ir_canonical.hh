#pragma once
#include "../statements/all_ir_statements.hh"
#include "../statements/ir_statement_list.hh"
#include "../expressions/all_ir_expressions.hh"
#include "statement_expression_list.hh"
#include "move_call.hh"
#include "exp_call.hh"
class IRCanonical
{
    static std::shared_ptr<IRStatementExpressionList> nopNull;
    
    static bool IsNop(std::shared_ptr<IRStatement>);

    static std::shared_ptr<IRStatement> Seq(std::shared_ptr<IRStatement> a, std::shared_ptr<IRStatement> b);

    static std::shared_ptr<IRStatement> DoStatement(std::shared_ptr<IRSequence>);

    static std::shared_ptr<IRStatement> DoStatement(std::shared_ptr<IRMove>);

    static std::shared_ptr<IRStatement> DoStatement(std::shared_ptr<IREvaluateExpression>);

    static std::shared_ptr<IRStatement> DoStatement(std::shared_ptr<IRStatement>);


    static std::shared_ptr<IRStatement> ReorderStatement(std::shared_ptr<IRStatement>);

    static std::shared_ptr<IREseq> ReorderExpression(std::shared_ptr<IRExpression>);

    static std::shared_ptr<IREseq> DoExpression(std::shared_ptr<IREseq>);

    static std::shared_ptr<IREseq> DoExpression(std::shared_ptr<IRExpression>);

    static bool Commute(std::shared_ptr<IRStatement> a, std::shared_ptr<IRExpression> b);

    static std::shared_ptr<IRStatementExpressionList> Reorder(std::shared_ptr<IRExpressionList>);

    static std::shared_ptr<IRStatementList> Linear(std::shared_ptr<IRSequence> s, std::shared_ptr<IRStatementList>);
    
    static std::shared_ptr<IRStatementList> Linear(std::shared_ptr<IRStatement> s, std::shared_ptr<IRStatementList>);
    
    public:
        static std::shared_ptr<IRStatementList> Linearize(std::shared_ptr<IRStatement> s);

};