#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "else_statement_node.hh"
#include "else_if_statement_node.hh"
#include "body_node.hh"
#include "variable_container.hh"
#include "../expressions/assignment_expression.hh"
#include "../../ir/statements/all_ir_statements.hh"
#include "../../translate/all_translate_expressions.hh"
#include "../../frame/label.hh"
#include <memory>
class IfStatementNode : public  Node, public BodyContainer
{
    
    public:
        std::shared_ptr<IExpressionable> expression;
        std::unique_ptr<BodyNode> body;
        std::vector<std::unique_ptr<ElseIfStatementNode>> else_ifs;
        std::unique_ptr<ElseStatementNode> else_;
        IfStatementNode(std::shared_ptr<IExpressionable> expr_in,std::unique_ptr<BodyNode> body_in,std::vector<std::unique_ptr<ElseIfStatementNode>> else_ifs_in, std::unique_ptr<ElseStatementNode> else_in, yy::location loc);

        ~IfStatementNode() override;
        bool Check() override;

        std::vector<std::shared_ptr<VariableSymbol>> GetVariables() override;

        std::vector<std::shared_ptr<ReturnStatementNode>> GetReturnNodes() override;


        std::shared_ptr<IRStatement> TranslateToIR() override;
};