#pragma once
#include "node.hh"
#include "statement_list_node.hh"
#include "../expressions/assignment_expression.hh"
#include "../expressions/function_call.hh"
#include "class_definition_node.hh"
#include "variable_container.hh"
#include "../../ir/statements/all_ir_statements.hh"
#include <vector>
#include <memory>

class ConstructorDefinitionNode;

class BodyNode : public Node, public BodyContainer
{
    std::vector<std::shared_ptr<Node>> statements;
    
    // Statements that needs to be printed at the start of the body (function pointer movement etc.)
    std::vector<std::shared_ptr<IRStatement>> priorityStatements;
    public:
    BodyNode(std::vector<std::shared_ptr<Node>>);
    BodyNode() = default;

    BodyNode(BodyNode&& other);
    public:
        ~BodyNode() override = default;

        // Will add the statements to start of the body
        void AddStatements(std::vector<std::shared_ptr<IRStatement>>);

        bool Check() override;
        bool isEmpty();
        std::vector<std::shared_ptr<Node>> GetStatements();

        // Will return true if the given statement can be used inside a body
        bool StatementIsValid(const std::shared_ptr<Node> statement);
        std::vector<std::shared_ptr<VariableSymbol>> GetVariables() override;
        std::vector<std::shared_ptr<ReturnStatementNode>> GetReturnNodes() override;

        std::shared_ptr<IRStatement> TranslateToIR() override;

};