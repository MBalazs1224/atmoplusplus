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
        auto classSymbol = std::dynamic_pointer_cast<ClassSymbol>(exp_right->GetType());
        if (!classSymbol)
        {
           Error::ShowError(Error::FormatString("ONly class types can appear on the right side of member access (inside) expression! (received '%s')",exp_right->GetType()->ToString().c_str()),exp_right->location);
        }
        
        classSymbol->Check();
        if (auto id = std::dynamic_pointer_cast<Identifier>(exp_left))
        {
            auto variable = classSymbol->GetVariable(id->name);
            if(!variable)
            {
                Error::ShowError(Error::FormatString("Cannot find '%s' inside '%s'",variable->name.c_str(),classSymbol->ToString().c_str()),variable->location);
            }
            id->SetElement(variable);
        }
        else if(auto functionCall = std::dynamic_pointer_cast<FunctionCall>(exp_left))
        {
            auto function = classSymbol->GetFunction(functionCall->name_for_function);
            if (!function)
            {
                Error::ShowError(Error::FormatString("Cannot find '%s' inside '%s'",functionCall->name_for_function.c_str(),classSymbol->ToString().c_str()),functionCall->location);
            }
            else
            {
                functionCall->SetFunction(function);
            }
            
        }
        else
        {
            Error::ShowError("Only function calls and variables can appear on the left side of the member access (inside) expression!",exp_left->location);
        }
    }
};
