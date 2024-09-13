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
        VariableDefinitionNode( std::shared_ptr<VariableSymbol> var_in, std::shared_ptr<IExpressionable> exp_in, yy::location loc)
        : Node(loc), variable(std::move(var_in)), expression(std::move(exp_in))
        {

        }
        ~VariableDefinitionNode() override = default;
        bool Check() override
        {
            // If the expression is not set, it means that there is no initalizing value
            if(expression)
            {
                if(!expression->Check())
            {
                return false;
            };
                auto exp_type = expression->GetType();
                auto var_type = variable->GetType();

                if (var_type->NotEquals(exp_type))
                {
                    Error::ShowError(Error::FormatString("Type mismatch between variable and initializing value! (Variable: '%s', Value: '%s')",var_type->ToString().c_str(),exp_type->ToString().c_str()),this->location);
                    return false;
                }
            }
            return true;

        }

        std::shared_ptr<VariableSymbol> GetVariable()
        {
            return variable;
        }
};