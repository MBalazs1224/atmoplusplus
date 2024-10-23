#pragma once
#include "one_operand_expression.hh"
#include "identifier.hh"
#include "member_access_expression.hh"
#include "../../symboltable/symbols/symbolfunction.hh"
#include <vector>
#include <memory>
class FunctionCall : public OneOperandExpression
{
    private:
        // It also has an expression that can be an identifier or a member access 

        // The function that will be called if the expression passed in was correct (used for type checking)
        std::shared_ptr<FunctionSymbol> function;

        // Will return false if there is a problem with the arguments, otherwise true
        bool CheckArguments();

        bool CheckElement();

        // Will return the function element from the identifier or the member access
        std::shared_ptr<SymbolTableElement> GetElementFromExpression();
    public:

        std::vector<std::shared_ptr<IExpressionable>> arguments;

        FunctionCall(std::shared_ptr<IExpressionable> func_in, std::vector<std::shared_ptr<IExpressionable>> args_in,yy::location loc);

        std::shared_ptr<Type> GetType() override;
        ~FunctionCall() override = default;

        // Check if the function and arguments are valid
        bool Check() override;
};
