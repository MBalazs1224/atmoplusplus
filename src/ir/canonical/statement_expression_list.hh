#pragma once
#include <memory>
#include "../statements/ir_statement.hh"
#include "../expressions/ir_expression_list.hh"

class IRStatementExpressionList
{
    public:
        std::shared_ptr<IRStatement> statement;
        std::shared_ptr<IRExpressionList> expressions;

        IRStatementExpressionList(std::shared_ptr<IRStatement> stmt, std::shared_ptr<IRExpressionList> exps) : statement(stmt), expressions(exps) {}
};