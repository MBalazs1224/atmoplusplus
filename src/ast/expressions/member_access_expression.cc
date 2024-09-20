#include "member_access_expression.hh"
#include "../../symboltable/symbols/symbolfunction.hh"
#include "../../symboltable/symbols/symbolclass.hh"
#include "identifier.hh"
#include "../../error/error.hh"
#include "function_call.hh"

bool MemberAccessExpression::Check()
{
        // Member access can be chained so the right side of the expression can be a member access (altought the left side of it the inner expression must be class type nad the get type function will check that later)
        if (auto memberAccess = std::dynamic_pointer_cast<MemberAccessExpression>(exp_right))
        {
            if(!memberAccess->Check())
            {
                return false;
            }
        }


        auto classSymbol = std::dynamic_pointer_cast<ClassSymbol>(exp_right->GetType());
        if (!classSymbol)
        {
           Error::ShowError(Helper::FormatString("Only class types can appear on the right side of member access (inside) expression! (received '%s')",exp_right->GetType()->ToString().c_str()),exp_right->location);
           return false;
        }
        
        classSymbol->Check();
        if (auto id = std::dynamic_pointer_cast<Identifier>(exp_left))
        {
            auto variable = classSymbol->GetVariable(id->name);
            if(!variable)
            {
                Error::ShowError(Helper::FormatString("Cannot find '%s' inside '%s'",id->name.c_str(),classSymbol->ToString().c_str()),this->location);
                return false;
            }
            if (!variable->IsReachableOutside())
            {
                Error::ShowError(Helper::FormatString("'%s' is inaccessible due to it's protection level! ('%s')",variable->name.c_str(), variable->GetAttribute()->ToString().c_str()),exp_left->location);
                return false;
            }
            
            id->SetElement(variable);
        }
        else if(auto functionCall = std::dynamic_pointer_cast<FunctionCall>(exp_left))
        {
            auto function = classSymbol->GetFunction(functionCall->name_for_function);
            if (!function)
            {
                Error::ShowError(Helper::FormatString("Cannot find '%s' inside '%s'",functionCall->name_for_function.c_str(),classSymbol->ToString().c_str()),this->location);
                return false;
            }
            if (!function->IsReachableOutside())
            {
                Error::ShowError(Helper::FormatString("'%s' is inaccessible due to it's protection level! ('%s')",function->name.c_str(), function->GetAttribute()->ToString().c_str()),exp_left->location);
                return false;
            }
            functionCall->SetFunction(function);
            functionCall->Check();
            
        }
        else if(auto memberAccess = std::dynamic_pointer_cast<MemberAccessExpression>(exp_left))
        {
        
            if(!memberAccess->Check())
            {
                return false;
            }
        }
        else
        {
            Error::ShowError("Only function calls and variables can appear on the left side of the member access (inside) expression!",exp_left->location);
            return false;
        }

    return true;

}