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
        //TODO: Implement Function call expression checking
        return true;
    }
    // Set the function to be called, if it wasn't found originally
    void SetFunction(std::shared_ptr<FunctionSymbol> element)
    {
        this->identifier->SetElement(element);
    }
};
