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
    if(!identifier->Check())
    {
        return false;
    }
    // Show error if the found element is not a function
    function = std::dynamic_pointer_cast<FunctionSymbol>(identifier->GetElement());
    if (!function)
    {
        Error::ShowError("Only functions can be called!",identifier->location);
        return false;
    }
    return true;
}

bool FunctionCall::CheckArguments()
{
    auto wanted_arguments = function->GetArguments();
    if(wanted_arguments.size() != arguments.size())
    {
        Error::ShowError(Helper::FormatString("Wrong number of parameters while calling '%s'! (wanted: %i, received: %i)", identifier->name.c_str(), wanted_arguments.size(), arguments.size()),this->location);
        return false;
    }
    // Check if the given parameters' type match with the wanted ones
    for (size_t i = 0; i < wanted_arguments.size(); i++)
    {
        if (!arguments[i]->Check())
        {
            return false;
        }
        
        auto wanted_argument_type = wanted_arguments[i]->GetType();
        auto current_argument_type = arguments[i]->GetType(); 
        if (wanted_argument_type->NotEquals(current_argument_type))
        {
            Error::ShowError(Helper::FormatString("Type mismatch between the %i. parameter to call '%s'! (wanted: '%s', received: '%s')",i+1, identifier->name.c_str(),wanted_argument_type->ToString().c_str(),current_argument_type->ToString().c_str()),arguments[i]->location);
            return false;
        }
    }
    return true;
}

FunctionCall::FunctionCall(std::shared_ptr<Identifier> func_in, std::vector<std::shared_ptr<IExpressionable>> args_in,yy::location loc, std::string name) : IExpressionable(loc)
{
    arguments = args_in;
    identifier = func_in;
    name_for_function = name;
}

std::shared_ptr<Type> FunctionCall::GetType() 
{
   // The semantic analyzer already checked if the function is valid so it cannot be null
   return identifier->GetType();
}

void FunctionCall::SetFunction(std::shared_ptr<FunctionSymbol> element)
{
    this->identifier->SetElement(element);
}