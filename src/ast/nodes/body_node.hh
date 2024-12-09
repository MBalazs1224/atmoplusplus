#pragma once
#include "node.hh"
#include "statement_list_node.hh"
#include "../expressions/assignment_expression.hh"
#include "../expressions/function_call.hh"
#include "class_definition_node.hh"
#include "variable_container.hh"
#include <vector>
#include <memory>

class ConstructorDefinitionNode;

class BodyNode : public Node, public VariableContainer
{
    std::vector<std::shared_ptr<Node>> statements;
    public:
    BodyNode(std::vector<std::shared_ptr<Node>>);
    BodyNode() = default;

    BodyNode(BodyNode&& other);
    public:
        ~BodyNode() override = default;

        bool Check() override;
        bool isEmpty();
        std::vector<std::shared_ptr<Node>> GetStatements();

        // Will return true if the given statement can be used inside a body
        bool StatementIsValid(const std::shared_ptr<Node> statement);
        std::vector<std::shared_ptr<VariableSymbol>> GetVariables() override;

        std::shared_ptr<IRStatement> TranslateToIR() override;

};