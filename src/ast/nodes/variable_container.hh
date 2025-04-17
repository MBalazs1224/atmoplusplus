#pragma once
#include <memory>
#include <vector>
#include "../..//symboltable/symbols/symbolvariable.hh"
#include "return_statement_node.hh"

// Represents a node that can contain variables inside it
class BodyContainer
{
    public:
        virtual std::vector<std::shared_ptr<VariableSymbol>> GetVariables() = 0;

        virtual std::vector<std::shared_ptr<ReturnStatementNode>> GetReturnNodes() = 0;
};