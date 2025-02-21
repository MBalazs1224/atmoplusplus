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
                Error::ShowError(Helper::FormatString("'%s' is inaccessible due to it's protection level!"),left->location);
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
    
    return left->GetType();
}

std::shared_ptr<TranslateExpression> MemberAccessExpression::TranslateExpressionToIr()
{
    // The right hand side will always yield the location of the object
    auto locationOfObject = right->TranslateExpressionToIr()->ToValueExpression();

    // The left expression will always be an identifier either a variable or function (and the Check function checked that it's valid)
    auto leftID = std::dynamic_pointer_cast<Identifier>(left);

    auto element = leftID->GetElement();

    // The check functions already checked that the identifier validly points to a class member so  we don't have to check it again
    auto classMember = std::dynamic_pointer_cast<ClassMember>(element);

    auto finalOffset = classMember->access->AsExpression(
        locationOfObject
    );

    return std::make_shared<TranslateValueExpression>(finalOffset);
}

std::shared_ptr<IRStatement> MemberAccessExpression::TranslateToIR()
{
    //TODO: "Implement MemberAccessExpression to ir"
    return nullptr;
}