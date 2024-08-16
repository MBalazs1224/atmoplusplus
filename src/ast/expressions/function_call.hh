#pragma once
#include "expression.hh"
#include "../../symboltable/symboltableelement.hh"
#include <vector>
#include <memory>
class FunctionCall : public Expression
{
    private:
        std::vector<std::shared_ptr<IExpressionable>> arguments;
        std::shared_ptr<SymbolTableElement> function;

    public:
        //FIXME: function call doesn't need expressions
        FunctionCall(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in, std::shared_ptr<SymbolTableElement> func_in, std::vector<std::shared_ptr<IExpressionable>> args_in,yy::location loc) : Expression(std::move(left_in),std::move(right_in),loc)
        {
            arguments = args_in;
            function = func_in;
        }
        ~FunctionCall() override = default;
};