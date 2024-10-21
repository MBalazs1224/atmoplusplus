#include "variable_definition_node.hh"
#include "../../error/error.hh"
#include "../../helper/helper.hh"

VariableDefinitionNode::VariableDefinitionNode(std::shared_ptr<VariableSymbol> var_in, std::shared_ptr<IExpressionable> exp_in, yy::location loc, std::vector<std::shared_ptr<IExpressionable>> args)
    : Node(loc), variable(std::move(var_in)), expression(std::move(exp_in)), arguments_to_constructor(std::move(args))
{
    // There must be a variable
    assert(variable);
}

bool VariableDefinitionNode::VariableIsClass()
{
    return std::dynamic_pointer_cast<ClassSymbol>(variable->GetType()) != nullptr;
}

bool VariableDefinitionNode::CheckConstructor()
{
    auto class_symbol = std::dynamic_pointer_cast<ClassSymbol>(variable->GetType());

    auto constructors = class_symbol->GetConstructorsWithParametersMatching(arguments_to_constructor);

    if (constructors.empty())
    {
        Error::ShowError(Helper::FormatString("No constructor found for the given parameters! (Class: '%s')", class_symbol->name.c_str()), this->location);
        return false;
    }
    else if (constructors.size() > 1)
    {
        Error::ShowError(Helper::FormatString("Ambiguous constructor call! (Class: '%s')", class_symbol->name.c_str()), this->location);
        return false;
    }

    // Save the constructor for later use
    constructor = constructors[0];
    return true;
}

bool VariableDefinitionNode::Check()
{
    // If the expression is not set it means that there is no initialization value
    if (expression)
    {
        // You cannot pass arguments to a constructor and initialize it at the same time

        if (arguments_to_constructor.size() > 0)
        {
            Error::ShowError("Cannot pass arguments to a variable and initialize it at the same time!", this->location);
            return false;
        }

        if (!expression->Check())
        {
            return false;
        }

        auto exp_type = expression->GetType();
        auto var_type = variable->GetType();

        if (exp_type->NotCompatible(var_type))
        {
            Error::ShowError(Helper::FormatString("The type of initialization value must be compatible with the variable's type! (Variable: '%s', Value: '%s')", var_type->ToString().c_str(), exp_type->ToString().c_str()), this->location);
            return false;
        }
        
    }
    // If the variable is a class and we don't have an initializing value,  we need o theck if it correctly calls the constructor
    else if (VariableIsClass())
    {
        return CheckConstructor();
    }

    else
    {
        if (arguments_to_constructor.size() > 0)
        {
            Error::ShowError("Cannot pass arguments to a non-class variable!", this->location);
            return false;
        }
    }
    return true;
}

std::shared_ptr<VariableSymbol> VariableDefinitionNode::GetVariable()
{
    return variable;
}