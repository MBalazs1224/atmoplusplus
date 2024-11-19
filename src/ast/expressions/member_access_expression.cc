#include "member_access_expression.hh"
#include "../../symboltable/symbols/symbolfunction.hh"
#include "../../symboltable/symbols/symbolclass.hh"
#include "identifier.hh"
#include "../../error/error.hh"
#include "function_call.hh"

bool MemberAccessExpression::Check()
{
        // Member access can be chained so the right side of the expression can be a member access, so wee need to checked that first


        if(!right->Check())
        {
            return false;
        }



        auto classSymbol = std::dynamic_pointer_cast<ClassSymbol>(right->GetType());
        if (!classSymbol)
        {
           Error::ShowError(Helper::FormatString("Only class types can appear on the right side of member access (inside) expression! (received '%s')",right->GetType()->ToString().c_str()),right->location);
           return false;
        }
        
        classSymbol->Check();

        if (auto id = std::dynamic_pointer_cast<Identifier>(left))
        {
            auto element = classSymbol->GetElement(id->name);
            if(!element)
            {
                Error::ShowError(Helper::FormatString("Cannot find '%s' inside '%s'",id->name.c_str(),classSymbol->ToString().c_str()),this->location);
                return false;
            }
            if (!element->IsReachableOutside())
            {
                Error::ShowError(Helper::FormatString("'%s' is inaccessible due to it's protection level! ('%s')",element->name.c_str(), element->GetAttribute()->ToString().c_str()),left->location);
                return false;
            }
            
            id->SetElement(element);

            // Save the identifier for later use
            this->identifier = id;
        }
        else
        {
            Error::ShowError("Only identifiers can appear on the left side of the member access (inside) expression!",left->location);
            return false;
        }

    return true;

}

std::shared_ptr<SymbolTableElement> MemberAccessExpression::GetElement()
{
    return std::dynamic_pointer_cast<SymbolTableElement>(identifier->GetElement());
}


MemberAccessExpression::MemberAccessExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : TwoOperandExpression(left_in,right_in,loc)
{
}


std::shared_ptr<Type> MemberAccessExpression::GetType()
{
    // FIXME: Member access might needs it's unique type
    return left->GetType();
}