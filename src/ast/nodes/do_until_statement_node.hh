#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "variable_container.hh"
#include "body_node.hh"
#include <memory>
#include <iostream>

class DoUntilStatementNode : public Node, public VariableContainer
{

    public:
        std::unique_ptr<BodyNode> body;
        std::shared_ptr<IExpressionable> expression;

        DoUntilStatementNode(std::shared_ptr<IExpressionable> expression_in,std::unique_ptr<BodyNode> body_in, yy::location loc);

        ~DoUntilStatementNode() override;

        bool Check() override;
        std::vector<std::shared_ptr<VariableSymbol>> GetVariables() override;
};