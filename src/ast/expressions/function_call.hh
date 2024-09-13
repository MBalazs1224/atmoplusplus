#pragma once
#include "expressionable.hh"
#include "../../symboltable/symboltableelement.hh"
#include <vector>
#include <memory>
class FunctionCall : public IExpressionable
{
    private:
        std::vector<std::shared_ptr<IExpressionable>> arguments;
        std::shared_ptr<SymbolTableElement> function;
    public:
    // It will be set to the wanted function if the parser cannot find it (because it's in a class etc.)
	    std::string name_for_function; 
        FunctionCall(std::shared_ptr<SymbolTableElement> func_in, std::vector<std::shared_ptr<IExpressionable>> args_in,yy::location loc, std::string name) : IExpressionable(loc)
        {
            arguments = args_in;
            function = func_in;
		name_for_function = name;
        }
    std::shared_ptr<Type> GetType() override
    {
       // If the function was found we give back it's return type
       if (function)
       {
        return function->GetType();
       }
       // TODO: Resolvefunction if it was not found by the parser
        return nullptr;
    }
        ~FunctionCall() override = default;

    void Check() override
    {
        //TODO: Implement Function call expression checking
    }
    // Set the function to be called, if it wasn't found originally
    void SetFunction(std::shared_ptr<SymbolTableElement> element)
    {
        function = element;
    }
};
