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
    return function->TranslateToIR();
}

void ConstructorDefinitionNode::AddStatementsToBody(std::vector<std::shared_ptr<IRStatement>> stmt)
{
    this->function->body->AddStatements(std::move(stmt));
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

    return IRExpressionList::CreateFromVector(expressions);
    
}


void ConstructorDefinitionNode::TranslateToIRWithGivenParemeter(std::shared_ptr<IRExpression> locationOfClassObject, std::shared_ptr<IRExpressionList> paramsToThisConstructor, std::vector<std::shared_ptr<IRStatement>>& statements)
{

    // If there is a parent constructor we need to evaluate that first
    if(constructorOfParent)
    {
        constructorOfParent->TranslateToIRWithGivenParemeter(
            locationOfClassObject,
            TranslateArgumentsToParentToIR(),
            statements
        );
    }


    // Get the location of the function

    auto functionLocation = function->TranslateExpressionToIr()->ToValueExpression();

    // The first parameter must be the location of the object

    auto fullParameterList = std::make_shared<IRExpressionList>();

    fullParameterList->expression = locationOfClassObject;
    fullParameterList->next = paramsToThisConstructor;


    // Call this constructor with the given parameters

    auto callThisConstructor = std::make_shared<IRCall>(
        functionLocation,
        fullParameterList,
        this->function->ConvertParameterLocationToList(),
        false, // The return value is not needed,
        DataSize::QWord // Will be a pointer
    );

    auto evaulateThisConstructor = std::make_shared<IREvaluateExpression>(
        callThisConstructor
    );


    statements.push_back(evaulateThisConstructor);

}
