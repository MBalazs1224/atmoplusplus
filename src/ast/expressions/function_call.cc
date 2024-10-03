#include "function_call.hh"
bool FunctionCall::Check()
{       
    if(!CheckElement() ||!CheckArguments())
    {
        return false;
    }
    return true;
}

bool FunctionCall::CheckElement()
{
    if(!expression->Check())
    {
        return false;
    }

    function = std::dynamic_pointer_cast<FunctionSymbol>(GetElementFromExpression());

    // Show error if the found element is not a function
    if(!function)
    {
        Error::ShowError("Only functions can be called!",expression->location);
        return false;
    }
    return true;
}

bool FunctionCall::CheckArguments()
{

    // Check if the number of parameters match
    // function cannot be null at this point because CheckElement checked that it's a correct function

    const auto wanted_arguments = function->GetArguments();
    if(wanted_arguments.size() != arguments.size())
    {
        Error::ShowError(Helper::FormatString("Wrong number of parameters while calling '%s'! (wanted: %i, received: %i)", function->name.c_str(), wanted_arguments.size(), arguments.size()),this->location);
        return false;
    }
    // Check if the given parameters    ' type match with the wanted ones
    for (size_t i = 0; i < wanted_arguments.size(); i++)
    {
        if (!arguments[i]->Check())
        {
            return false;
        }
        
        auto wanted_argument_type = wanted_arguments[i]->GetType();
        auto current_argument_type = arguments[i]->GetType(); 
        if (wanted_argument_type->NotCompatible(current_argument_type))
        {
            Error::ShowError(Helper::FormatString("Type mismatch between the %i. parameter to call '%s'! (wanted: '%s', received: '%s')",i+1, function->name.c_str(),wanted_argument_type->ToString().c_str(),current_argument_type->ToString().c_str()),arguments[i]->location);
            return false;
        }
    }
    return true;
}

FunctionCall::FunctionCall(std::shared_ptr<IExpressionable> func_in, std::vector<std::shared_ptr<IExpressionable>> args_in,yy::location loc) : IExpressionable(loc)
{
    assert(func_in);

    arguments = args_in;
    expression = func_in;
}

std::shared_ptr<Type> FunctionCall::GetType() 
{
   // The semantic analyzer already checked if the function is valid so it cannot be null
   return expression->GetType();
}

std::shared_ptr<SymbolTableElement> FunctionCall::GetElementFromExpression()
{
    // If the expression is a MemberContainer, return the element referenced by it
    auto memberContainer = std::dynamic_pointer_cast<MemberContainer>(expression);
    if (memberContainer)
    {
        return memberContainer->GetElement();
    }
    return nullptr;
}