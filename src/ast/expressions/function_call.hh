#pragma once
#include "expressionable.hh"
#include "identifier.hh"
#include <vector>
#include <memory>
class FunctionCall : public IExpressionable
{
    private:
        std::vector<std::shared_ptr<IExpressionable>> arguments;
        std::shared_ptr<Identifier> identifier;

        // The function after the identifier was found in the symboltable, nullptr by default
        std::shared_ptr<FunctionSymbol> function = nullptr;

    // Will return false if there is a problem with the arguments, otherwise true
    bool CheckArguments()
    {
        auto wanted_arguments = function->GetArguments();
        if(wanted_arguments.size() != arguments.size())
        {
            Error::ShowError(Error::FormatString("Wrong number of parameters while calling '%s'! (wanted: %i, received: %i)", identifier->name.c_str(), wanted_arguments.size(), arguments.size()),this->location);
            return false;
        }
        // Check if the given parameters' type match with the wanted ones
        for (size_t i = 0; i < wanted_arguments.size(); i++)
        {
            if (!arguments[i]->Check())
            {
                return false;
            }
            
            auto wanted_argument_type = wanted_arguments[i]->GetType();
            auto current_argument_type = arguments[i]->GetType(); 
            if (wanted_argument_type->NotEquals(current_argument_type))
            {
                Error::ShowError(Error::FormatString("Type mismatch between the %i. parameter to call '%s'! (wanted: '%s', received: '%s')",i+1, identifier->name.c_str(),wanted_argument_type->ToString().c_str(),current_argument_type->ToString().c_str()),arguments[i]->location);
                return false;
            }
        }

        return true;
    }

    bool CheckElement()
    {
        if(!identifier->Check())
        {
            return false;
        }

        // Show error if the found element is not a function
        function = std::dynamic_pointer_cast<FunctionSymbol>(identifier->GetElement());
        if (!function)
        {
            Error::ShowError("Only functions can be called!",identifier->location);
            return false;
        }
        return true;
    }
    public:
    // It will be set to the wanted function if the parser cannot find it (because it's in a class etc.)
	    std::string name_for_function; 
        FunctionCall(std::shared_ptr<Identifier> func_in, std::vector<std::shared_ptr<IExpressionable>> args_in,yy::location loc, std::string name) : IExpressionable(loc)
        {
            arguments = args_in;
            identifier = func_in;
		    name_for_function = name;
        }
    std::shared_ptr<Type> GetType() override
    {
       // The semantic analyzer already checked if the function is valid so it cannot be null
       return identifier->GetType();
    }
    ~FunctionCall() override = default;

    bool Check() override
    {
        
        if(!CheckElement() ||!CheckArguments())
        {
            return false;
        }

        //TODO: Implement Function call expression checking
        return true;
    }
    // Set the function to be called, if it wasn't found originally
    void SetFunction(std::shared_ptr<FunctionSymbol> element)
    {
        this->identifier->SetElement(element);
    }
};
