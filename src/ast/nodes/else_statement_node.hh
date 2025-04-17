#pragma once
#include "node.hh"
#include "body_node.hh"
#include "variable_container.hh"
#include <memory>
class ElseStatementNode : public  Node, public BodyContainer
{
    

    public:
        std::unique_ptr<BodyNode> body;
        ElseStatementNode(std::unique_ptr<BodyNode> body_in, yy::location loc);
        ~ElseStatementNode() override;
        bool Check() override;
        std::vector<std::shared_ptr<VariableSymbol>> GetVariables() override;

        std::vector<std::shared_ptr<ReturnStatementNode>> GetReturnNodes() override;


        std::shared_ptr<IRStatement> TranslateToIR() override;
};