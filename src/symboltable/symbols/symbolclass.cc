#include "symbolclass.hh"

ClassSymbol::ClassSymbol(std::vector<std::shared_ptr<Identifier>> parents_in, std::unique_ptr<BodyNode> body_in)
    : parents(std::move(parents_in)), body(body_in->GetStatements()) {}

std::shared_ptr<Type> ClassSymbol::GetType() {
    return shared_from_this();
}

bool ClassSymbol::Compatible(const std::shared_ptr<Type> other) {
    auto casted = std::dynamic_pointer_cast<ClassSymbol>(other);

    // The class i compatible with the other if they are the same or if the other class is a parent of this class

    return (casted && this->name == casted->name) || this->IsClassAParent(casted);
}

std::string ClassSymbol::ToString() {
    return Helper::FormatString("type class (%s)", name.c_str());
}
std::unordered_map<std::string, std::shared_ptr<VariableSymbol>> ClassSymbol::GetVariables() {
    return variables;
}

std::unordered_map<std::string, std::shared_ptr<FunctionSymbol>> ClassSymbol::GetFunctions() {
    return functions;
}

bool ClassSymbol::IsClassAParent(const std::shared_ptr<ClassSymbol>& other)
{
    // Wecan only have one class so it might be more effctive to change the loop to a normal check
    for (auto &&parent : parents)
    {


        auto parent_class = std::dynamic_pointer_cast<ClassSymbol>(parent->GetElement());

        // The check function already checked at this point that the identifiers are valid and that the parent is a class

        if (parent_class == other)
        {
            return true;
        }
    }

    return false;
    
}

const std::vector<std::shared_ptr<FunctionSymbol>> ClassSymbol::GetConstructorsWithParametersMatching(const std::vector<std::shared_ptr<IExpressionable>>& params_in)
{
    std::vector<std::shared_ptr<FunctionSymbol>> matching_constructors;

    for (auto &&constructor : constructors)
    {
        auto constructor_function = constructor->GetFunction();
        auto const_arguments = constructor_function->GetArguments();

        // If the number of parameters don't match, we can skip this constructor

        if (const_arguments.size() != params_in.size())
        {
            continue;
        }
        
        // Check if the given parameters' types match with the wanted ones

        for (size_t i = 0; i < const_arguments.size(); i++)
        {
            // The parameters can be complex expressions so we need to check them

            if (!params_in[i]->Check())
            {
                goto parameters_dont_match;
            }
            

            auto wanted_argument_type = const_arguments[i]->GetType();
            auto current_argument_type = params_in[i]->GetType();

            // If the types don't match, we can skip this constructor

            if (wanted_argument_type->NotCompatible(current_argument_type))
            {
                goto parameters_dont_match;
            }
        }
        matching_constructors.push_back(constructor_function);
        // FIXME: This might be implemented without a goto
        parameters_dont_match:
            continue;
    }

    return matching_constructors;
    
}

bool ClassSymbol::CheckParents()
{
    // Return true if there are no parents

    if (parents.empty())
    {
        return true;
    }
    
    if(parents.size() > 1)
    {
        Error::ShowError("A class can only have at most one parent!",this->location);
        return false;
    }

    // We need to check if the identifier to the parent is valid and inherit the variables and functions from the parent class

    auto parent_id = parents[0];

    // Check if the identifier to the parent class is valid

    if (!parent_id->Check())
    {
        return false;
    }

    // Check if the parent is actually a class

    auto parent_class = std::dynamic_pointer_cast<ClassSymbol>(parent_id->GetType());
    
    if (!parent_class)
    {
        Error::ShowError("Only classes can be used as a parent to another class!", parent_id->location);
        return false;
    }

    // Check if the parent class itself is valid

    if (!parent_class->Check())
    {
        return false;
    }
    
    // Insert the elements into the current class

    auto parent_variables = parent_class->GetVariables();
    variables.insert(parent_variables.begin(), parent_variables.end());
    auto parent_functions = parent_class->GetFunctions();
    functions.insert(parent_functions.begin(), parent_functions.end());

    return true;
}

bool ClassSymbol::IdentifierAlreadyDeclared(const std::string& id)
{
    // A variable cannot be declared with the same name as a function and vice versa

    return variables.find(id) != variables.end() || functions.find(id) != functions.end();
}

bool ClassSymbol::InsertFunction(const std::shared_ptr<FunctionDefinitionNode>& node)
{
    auto function = node->GetFunction();

    if (IdentifierAlreadyDeclared(function->name))
    {
        Error::ShowError(Helper::FormatString("Identifier '%s' has already been declared in this scope!", function->name.c_str()), node->location);
        return false;
    }
    
    functions[function->name] = function;

    return true;
}

bool ClassSymbol::InsertVariable(const std::shared_ptr<VariableDefinitionNode>& node)
{
    auto variable = node->GetVariable();

    if (IdentifierAlreadyDeclared(variable->name))
    {
        Error::ShowError(Helper::FormatString("Identifier '%s' has already been declared in this scope!", variable->name.c_str()), node->location);
        return false;
    }
    
    variables[variable->name] = variable;

    return true;
}

bool ClassSymbol::ProcessBody()
{
    // Show warning if body is empty

    if (body.empty())
    {
        Error::ShowWarning("Empty class definition!", this->location);
        return true;
    }
    
    for (auto &node : body)
    {

        if (auto variableDefinition = std::dynamic_pointer_cast<VariableDefinitionNode>(node))
        {
            // We need to check if the variable was defined correctly

            if (!variableDefinition->Check() || !InsertVariable(variableDefinition))
            {
                return false;
            }
        }
        else if (auto functionDefinition = std::dynamic_pointer_cast<FunctionDefinitionNode>(node))
        {
            // We need to check if the function was defined correctly

            if (!functionDefinition->Check() || !InsertFunction(functionDefinition))
            {
                return false;
            }
        }
        else if (auto constructorDefinition = std::dynamic_pointer_cast<ConstructorDefinitionNode>(node))
        {
            // We need to check if the constructor was defined correctly

            if (!constructorDefinition->Check())
            {
                return false;
            }
            // TODO: Implement constructor checking
            constructors.push_back(constructorDefinition);


        }
        else
        {
            Error::ShowError("Only variable and/or function definitions can appear at the top level of a class!", node->location);
            return false;
        }
    }
    return true;
}

bool ClassSymbol::CheckConstructors()
{
    // Generate empty constructor if no constructor is defined
    if(constructors.empty())
    {
        auto empty_constructor = std::make_shared<ConstructorDefinitionNode>();
        constructors.push_back(empty_constructor);
        
        Error::ShowWarning("No constructor defined for class, generating default one!",this->location);
        return true;
    }
    // Get the parent class if there is one, otherwise set it to nullptr, indicating there is no parent

    auto parent_class = !parents.empty() ? std::dynamic_pointer_cast<ClassSymbol>(parents[0]->GetType()) : nullptr;

    for (auto &&constructor : constructors)
    {
        // The constructor was checked inside the ProcessBody function so we don't need to check here

        // The children class must call the parent's constructor, so if there is a parent we need to check if the constructors are correctly chained

        if(parent_class)
        {
            auto arguments_for_parent_constructor = constructor->GetArgumentsForParentConstructor();

            auto parent_constructors = parent_class->GetConstructorsWithParametersMatching(arguments_for_parent_constructor);

            // FIXME: The constructors location is not set for some reason

            if (parent_constructors.empty())
            {
                Error::ShowError("No suitable parent constructor found for the given parameters!",constructor->location);
                return false;
            }
            
            if(parent_constructors.size() > 1)
            {
                Error::ShowError("Ambigous parent constructor call for the given parameters!",constructor->location);
                return false;
            }
            
        }
    }

    return true;
    
}

bool ClassSymbol::Check()
{
    if (alreadyChecked) {
        return checkedResult;
    }
    
    alreadyChecked = true;

    //FIXME: Saving the result in a bool looks a bit overcomplicated

    if(!CheckParents())
    {
        checkedResult = false;
        return false;
    }
    

    if(!ProcessBody())
    {
        checkedResult = false;
        return false;
    }


    if(!CheckConstructors())
    {
        checkedResult = false;
        return false;
    }


    
    

   
    checkedResult = true;
    return true;
}

std::shared_ptr<FunctionSymbol> ClassSymbol::GetFunction(const std::string& id) {
    return functions[id];
}

std::shared_ptr<VariableSymbol> ClassSymbol::GetVariable(const std::string& id) {
    return variables[id];
}

std::shared_ptr<SymbolTableElement> ClassSymbol::GetElement(const std::string& id) {

    // Check if the element is a variable if not return the id as a function

    auto variable = GetVariable(id);
    if (variable) {
        return variable;
    }
    return GetFunction(id);
}
