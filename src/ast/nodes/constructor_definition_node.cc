#include "constructor_definition_node.hh"

// Constructor: initializes the function symbol
ConstructorDefinitionNode::ConstructorDefinitionNode(std::shared_ptr<FunctionSymbol> function_in, std::vector<std::shared_ptr<IExpressionable>> args_in, yy::location loc)
    : Node(loc), arguments_to_parent(args_in), function(std::move(function_in)) 
{
}

ConstructorDefinitionNode::ConstructorDefinitionNode(yy::location location) : arguments_to_parent(), function(std::make_shared<FunctionSymbol>())  {
    this->location = location;
}

// Destructor: uses default since no custom cleanup is required
ConstructorDefinitionNode::~ConstructorDefinitionNode() = default;

// Check function: delegates the check operation to the function symbol
bool ConstructorDefinitionNode::Check()
{
    return function->Check();
}

// GetFunction function: returns the stored function symbol
std::shared_ptr<FunctionSymbol> ConstructorDefinitionNode::GetFunction()
{
    return function;
}

const std::vector<std::shared_ptr<IExpressionable>>& ConstructorDefinitionNode::GetArgumentsForParentConstructor()
{
    return arguments_to_parent;
}

std::shared_ptr<IRStatement> ConstructorDefinitionNode::TranslateToIR()
{
    throw std::logic_error("Constructor definitions shouldn't be translated to IR!");
}

std::shared_ptr<IRExpressionList> ConstructorDefinitionNode::TranslateArgumentsToParentToIR()
{
    std::vector<std::shared_ptr<IRExpression>> expressions;
    for (auto &&exp : arguments_to_parent)
    {
        expressions.push_back(
            exp->TranslateExpressionToIr()->ToValueExpression()
        );
    }

    return std::make_shared<IRExpressionList>(expressions);
    
}


std::shared_ptr<IRStatement> ConstructorDefinitionNode::TranslateToIRWithGivenParemeter(std::shared_ptr<IRExpression> locationOfClassObject, std::shared_ptr<IRExpressionList> paramsToThisConstructor)
{


    // Get the location of the function

    auto functionLocation = function->TranslateExpressionToIr()->ToValueExpression();

    // The first parameter must be the location of the object

    auto fullParameterList = std::make_shared<IRExpressionList>();

    fullParameterList->expression = locationOfClassObject;
    fullParameterList->next = paramsToThisConstructor;

    // Call this constructor with the given parameters

    auto callThisConstructor = std::make_shared<IRCall>(
        functionLocation,
        fullParameterList
    );

    auto evaulateThisConstructor = std::make_shared<IREvaluateExpression>(
        callThisConstructor
    );

    // If there exists a parent constructor call, then we need to call that constructor before calling this one, otherwise just call this first

    if(constructorOfParent)
    {
        auto parentConstructorIR = constructorOfParent->TranslateToIRWithGivenParemeter(
            locationOfClassObject,
            TranslateArgumentsToParentToIR()
        );

        return std::make_shared<IRSequence>(
            parentConstructorIR,
            evaulateThisConstructor
        );
    }

    return evaulateThisConstructor;

}
