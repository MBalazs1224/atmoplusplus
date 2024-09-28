#include "member_access_expression.hh"
#include "../../symboltable/symbols/symbolfunction.hh"
#include "../../symboltable/symbols/symbolclass.hh"
#include "identifier.hh"
#include "../../error/error.hh"
#include "function_call.hh"

bool MemberAccessExpression::Check()
{
        // Member access can be chained so the right side of the expression can be a member access, so wee need to checked that first, it can also be an identifier (to a variable etc.)


        if(!exp_right->Check())
        {
            return false;
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
            auto element = classSymbol->GetElement(id->name);
            if(!element)
            {
                Error::ShowError(Helper::FormatString("Cannot find '%s' inside '%s'",id->name.c_str(),classSymbol->ToString().c_str()),this->location);
                return false;
            }
            if (!element->IsReachableOutside())
            {
                Error::ShowError(Helper::FormatString("'%s' is inaccessible due to it's protection level! ('%s')",element->name.c_str(), element->GetAttribute()->ToString().c_str()),exp_left->location);
                return false;
            }
            
            id->SetElement(element);
        }
        else
        {
            Error::ShowError("Only identifiers can appear on the left side of the member access (inside) expression!",exp_left->location);
            return false;
        }

    return true;

}

std::shared_ptr<SymbolTableElement> MemberAccessExpression::GetElement()
{
    // This might need a rework if the casted thing doesn't keep it's original type (function or variable etc.)


    // TODO: This may needs a rework so we don't have to cast it to identifier (maybe store it in a private variable)

    // The left side of the member access is always an identifier, the Check function made sure it's valid
    auto identifier = std::dynamic_pointer_cast<Identifier>(exp_left);

    return std::dynamic_pointer_cast<SymbolTableElement>(identifier->GetElement());
}