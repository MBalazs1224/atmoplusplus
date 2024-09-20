#pragma once
#include "expressionable.hh"
#include "identifier.hh"
#include "../../symboltable/symbols/symbolfunction.hh"
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
        bool CheckArguments();

        bool CheckElement();
    public:

        // It will be set to the wanted function if the parser cannot find it (because it's in a class etc.)
	    std::string name_for_function; 

        FunctionCall(std::shared_ptr<Identifier> func_in, std::vector<std::shared_ptr<IExpressionable>> args_in,yy::location loc, std::string name);

        std::shared_ptr<Type> GetType() override;
        ~FunctionCall() override = default;

        // Check if the function and arguments are valid
        bool Check() override;

        // Set the function to be called, if it wasn't found originally
        void SetFunction(std::shared_ptr<FunctionSymbol> element);
};
