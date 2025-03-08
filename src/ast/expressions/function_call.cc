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
        if (current_argument_type->NotCompatible(wanted_argument_type))
        {
            Error::ShowError(Helper::FormatString("Type mismatch between the %i. parameter to call '%s'! (wanted: '%s', received: '%s')",i+1, function->name.c_str(),wanted_argument_type->ToString().c_str(),current_argument_type->ToString().c_str()),arguments[i]->location);
            return false;
        }
    }
    return true;
}

FunctionCall::FunctionCall(std::shared_ptr<IExpressionable> func_in, std::vector<std::shared_ptr<IExpressionable>> args_in,yy::location loc) : OneOperandExpression(func_in,loc)
{
    assert(func_in);

    arguments = args_in;
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

std::shared_ptr<IRExpressionList> FunctionCall::TranslateArgumentsToIR()
{
    // If there are no arguments then there will be an empty argumentList object (because the object is created before te loop) which will mess up the ir-printing, so we just return null explicitly in this case
    if (arguments.empty())
    {
        return nullptr;
    }

    std::shared_ptr<IRExpressionList> argumentsList = std::make_shared<IRExpressionList>();
    for (auto &&arg : arguments)
    {
        argumentsList->expression = arg->TranslateExpressionToIr()->ToValueExpression();
        argumentsList->next = std::make_shared<IRExpressionList>();
        argumentsList = argumentsList->next;
    }

    return argumentsList;
}

std::shared_ptr<TranslateExpression> FunctionCall::TranslateExpressionToIr()
{

    std::vector<std::shared_ptr<IRStatement>> statements;

    std::shared_ptr<IRExpressionList> argumentsList = TranslateArgumentsToIR();

    
    auto funcLocation = expression->TranslateExpressionToIr()->ToValueExpression();

    // Move the arguments to the location of the parameters

    // Will return the parameters, the name just wrong for now
    auto functionParams = this->function->GetArguments();


    // If the expression is a MemberAccessExpression then that means, that the function is inside a class, so the location of the object should be moved into RDI
    if(auto memberAccess = std::dynamic_pointer_cast<MemberAccessExpression>(expression))
    {

        // If the wanted function is a member access expression, then that means that the function is inside a class, so the first argument must be the this pointer (which's location is the right element of the member access)
        
        arguments.insert(
            arguments.begin(),
            memberAccess->GetRight()
        );
    }

    assert(functionParams.size() == arguments.size());


    for (size_t i = 0; i < this->arguments.size(); i++)
    {
        auto move = std::make_shared<IRMove>(
            functionParams[i]->TranslateExpressionToIr()->ToValueExpression(), // The param's location
            arguments[i]->TranslateExpressionToIr()->ToValueExpression() // The argument's value
        );

        statements.push_back(move);

    }
    
    

    auto functionCall = std::make_shared<IRCall>(funcLocation, argumentsList);

    
    auto funcEvaluated = std::make_shared<IREvaluateExpression>(functionCall);

    statements.push_back(funcEvaluated);


    
    
    // The function will return the return value inside RAX, so we need to evaluate the function and get RAX for the result
    
    auto eseq = std::make_shared<IREseq>(
        std::make_shared<IRTemp>(ReservedIrRegisters::RAX),
        std::make_shared<IRSequence>(
            statements
        )
    );
    

    return std::make_shared<TranslateValueExpression>(
        eseq
    );
}

std::shared_ptr<IRStatement> FunctionCall::TranslateToIR()
{
    auto dummyHead = std::make_shared<IRExpressionList>(); 
    auto current = dummyHead; 

    for (auto &&arg : arguments)
    {
        current->next = std::make_shared<IRExpressionList>();
        current = current->next;
        current->expression = arg->TranslateExpressionToIr()->ToValueExpression();
    }

    auto argumentsList = dummyHead->next;
     


    auto functionLocation = expression->TranslateExpressionToIr()->ToValueExpression();
    // Generate the function call
    auto functionCall = std::make_shared<IRCall>(functionLocation, argumentsList);


    // We only need to evaluate the function call
    return std::make_shared<IREvaluateExpression>(functionCall);
}