#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "../attributes/attribute.hh"
#include "../types/type.hh"
#include <memory>

class VariableDefinitionNode : public Node
{
    // Pointer to the created variable
    std::shared_ptr<VariableSymbol> variable;
    std::shared_ptr<IExpressionable> expression;

    public:
        VariableDefinitionNode( std::shared_ptr<VariableSymbol> var_in, std::shared_ptr<IExpressionable> exp_in)
        :  variable(std::move(var_in)), expression(std::move(exp_in))
        {

        }
        ~VariableDefinitionNode() override = default;
        void Check() override
        {
            //TODO: Implement variable definition node checking
        }
};