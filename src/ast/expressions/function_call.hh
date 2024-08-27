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
        FunctionCall(std::shared_ptr<SymbolTableElement> func_in, std::vector<std::shared_ptr<IExpressionable>> args_in,yy::location loc) : IExpressionable(loc)
        {
            arguments = args_in;
            function = func_in;
        }
        ~FunctionCall() override = default;
};
