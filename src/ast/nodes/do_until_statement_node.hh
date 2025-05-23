#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "variable_container.hh"
#include "body_node.hh"
#include "../expressions/assignment_expression.hh"
#include "../../ir/statements/all_ir_statements.hh"
#include "../../frame/label.hh"
#include "../../ir/expressions/all_ir_expressions.hh"
#include <memory>
#include <iostream>

class DoUntilStatementNode : public Node, public BodyContainer
{

    public:
        std::unique_ptr<BodyNode> body;
        std::shared_ptr<IExpressionable> expression;

        DoUntilStatementNode(std::shared_ptr<IExpressionable> expression_in,std::unique_ptr<BodyNode> body_in, yy::location loc);

        ~DoUntilStatementNode() override;

        bool Check() override;
        std::vector<std::shared_ptr<VariableSymbol>> GetVariables() override;
        std::vector<std::shared_ptr<ReturnStatementNode>> GetReturnNodes() override;


        std::shared_ptr<IRStatement> TranslateToIR() override;
};