#pragma once
#include "statement_node.hh"
#include "../expressions/expressionable.hh"
#include "../../symboltable/symboltableelement.hh"
#include <memory>
#include <iostream>

class VariableAssignmentNode : public StatementNode
{
    // Null if there is no symbol, might be other than variable
    std::shared_ptr<SymbolTableElement> variable;
    std::shared_ptr<IExpressionable> expression;

    public:
        VariableAssignmentNode(std::shared_ptr<SymbolTableElement> var, std::shared_ptr<IExpressionable> exp_in) : variable(var), expression(std::move(exp_in))
        {

        }
};