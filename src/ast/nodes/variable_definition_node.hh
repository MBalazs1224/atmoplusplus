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

    public:
        VariableDefinitionNode( std::shared_ptr<VariableSymbol> attribute_in)
        :  variable(std::move(attribute_in))
        {

        }
        ~VariableDefinitionNode() override = default;
        void Check() override
        {
            //TODO: Implement variable definition node checking
        }
};