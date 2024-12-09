#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "variable_container.hh"
#include "body_node.hh"
#include <memory>
class ElseIfStatementNode : public  Node, public VariableContainer
{


    public:
        std::shared_ptr<IExpressionable> expression;
        std::unique_ptr<BodyNode> body;
        ElseIfStatementNode(std::shared_ptr<IExpressionable> expression_in ,std::unique_ptr<BodyNode> body_in, yy::location loc);
        ~ElseIfStatementNode() override;
        bool Check() override;
        std::vector<std::shared_ptr<VariableSymbol>> GetVariables() override;

        std::shared_ptr<IRStatement> TranslateToIR() override;
};