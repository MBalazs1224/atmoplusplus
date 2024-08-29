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
	// It will be set to the wanted function if the parser cannot find it (because it's in a class etc.)
	std::string name_for_function; 
    public:
        FunctionCall(std::shared_ptr<SymbolTableElement> func_in, std::vector<std::shared_ptr<IExpressionable>> args_in,yy::location loc, std::string name) : IExpressionable(loc)
        {
            arguments = args_in;
            function = func_in;
		name_for_function = name;
        }
        ~FunctionCall() override = default;
};
