#pragma once
#include "body_node.hh"
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "variable_container.hh"
#include "../expressions/assignment_expression.hh"
#include "../../ir/statements/all_ir_statements.hh"
#include "../../frame/label.hh"
#include "../../translate/all_translate_expressions.hh"
#include <memory>
#include <iostream>

class UntilStatementNode : public Node, public BodyContainer
{


    public:
        std::unique_ptr<BodyNode> body;
        std::shared_ptr<IExpressionable> expression;
        UntilStatementNode(std::shared_ptr<IExpressionable> expression_in,std::unique_ptr<BodyNode> body_in,yy::location loc);
        ~UntilStatementNode() override;
        bool Check() override;

        std::vector<std::shared_ptr<VariableSymbol>> GetVariables() override;

        std::vector<std::shared_ptr<ReturnStatementNode>> GetReturnNodes() override;


        std::shared_ptr<IRStatement> TranslateToIR() override;
};