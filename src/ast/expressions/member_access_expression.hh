#pragma once
#include "expressionable.hh"
class MemberAccessExpression : public IExpressionable
{
    std::shared_ptr<IExpressionable> exp_left;
	std::shared_ptr<IExpressionable> exp_right;
    public:
    MemberAccessExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : IExpressionable(loc), exp_left(left_in),exp_right(right_in)
    {

    }
        std::shared_ptr<Type> GetType() override
    {
        // FIXME: Member access might needs it's unique type
        return exp_left->GetType();
    }
    ~MemberAccessExpression() override = default;

    void Check() override
    {
        // The type of the right side of the expression must be type class
        auto classSymbol = std::dynamic_pointer_cast<ClassSymbol>(exp_right->GetType());

        if (!classSymbol)
        {
            Error::ShowError("Only class types can be on the right side of the member access (inside) expression!",exp_right->location);
            return;
        }

        classSymbol->Check();
        


        // Only variables and functions can be accessed with member access

        // The parser might not have found the wanted variable or function so we have to go inside the class and find it manually

        if (auto variable = std::dynamic_pointer_cast<VariableSymbol>(exp_left))
        {
            /* code */
        }
        else if(auto functionCall = std::dynamic_pointer_cast<FunctionCall>(exp_left))
        {
            auto functionCalled = classSymbol->GetFunction(functionCall->name_for_function);

            functionCall->SetFunction(functionCalled);

            // Has to check if the function call is valid after finding the correct function
            functionCall->Check();

        }
        else
        {
            
        }
    }
};
