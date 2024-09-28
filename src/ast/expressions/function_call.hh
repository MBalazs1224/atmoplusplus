#pragma once
#include "expressionable.hh"
#include "identifier.hh"
#include "member_access_expression.hh"
#include "../../symboltable/symbols/symbolfunction.hh"
#include <vector>
#include <memory>
class FunctionCall : public IExpressionable
{
    private:
        std::vector<std::shared_ptr<IExpressionable>> arguments;
        // The expression that will be called (should be identifier or member access)
        std::shared_ptr<IExpressionable> expression;

        // The function that will be called if the expression passed in was correct (used for type checking)
        std::shared_ptr<FunctionSymbol> function;

        // Will return false if there is a problem with the arguments, otherwise true
        bool CheckArguments();

        bool CheckElement();

        // Will return the function element from the identifier or the member access
        std::shared_ptr<SymbolTableElement> GetElementFromExpression();
    public:
        FunctionCall(std::shared_ptr<IExpressionable> func_in, std::vector<std::shared_ptr<IExpressionable>> args_in,yy::location loc);

        std::shared_ptr<Type> GetType() override;
        ~FunctionCall() override = default;

        // Check if the function and arguments are valid
        bool Check() override;
};
