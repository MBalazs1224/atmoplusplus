#include "variable_definition_node.hh"
#include "../../error/error.hh"
#include "../../helper/helper.hh"

VariableDefinitionNode::VariableDefinitionNode(std::shared_ptr<VariableSymbol> var_in, std::shared_ptr<IExpressionable> exp_in, yy::location loc, std::vector<std::shared_ptr<IExpressionable>> args)
    : Node(loc), variable(std::move(var_in)), arguments_to_constructor(std::move(args))
{
    expression = std::move(exp_in);
    // There must be a variable
    assert(variable);
}

bool VariableDefinitionNode::VariableIsClass()
{
    return std::dynamic_pointer_cast<ClassSymbol>(variable->GetType()) != nullptr;
}

bool VariableDefinitionNode::CheckConstructor()
{
    auto class_symbol = std::dynamic_pointer_cast<ClassSymbol>(variable->GetType());

    auto constructors = class_symbol->GetConstructorsWithParametersMatching(arguments_to_constructor);

    if (constructors.empty())
    {
        Error::ShowError(Helper::FormatString("No constructor found for the given parameters! (Class: '%s')", class_symbol->name.c_str()), this->location);
        return false;
    }
    else if (constructors.size() > 1)
    {
        Error::ShowError(Helper::FormatString("Ambiguous constructor call! (Class: '%s')", class_symbol->name.c_str()), this->location);
        return false;
    }

    // Save the constructor for later use

    constructor = constructors[0];
    return true;
}

bool VariableDefinitionNode::Check()
{
    if(!variable->Check())
    {
        return false;
    }
    // If the expression is not set it means that there is no initialization value
    if (expression)
    {
        // You cannot pass arguments to a constructor and initialize it at the same time

        if (arguments_to_constructor.size() > 0)
        {
            Error::ShowError("Cannot pass arguments to a variable and initialize it at the same time!", this->location);
            return false;
        }

        if (!expression->Check())
        {
            return false;
        }

        auto exp_type = expression->GetType();
        auto var_type = variable->GetType();

        if (exp_type->NotCompatible(var_type))
        {
            Error::ShowError(Helper::FormatString("The type of initialization value must be compatible with the variable's type! (Variable: '%s', Value: '%s')", var_type->ToString().c_str(), exp_type->ToString().c_str()), this->location);
            return false;
        }
        
    }
    else if(auto array = std::dynamic_pointer_cast<Array>(variable->GetType()))
    {
        if(!array->number_of_elements)
        {
            Error::ShowError("The number of elements of an array must be specified!",this->location);
            return false;
        }

        auto element_expression = array->number_of_elements->GetType();
        if(!std::dynamic_pointer_cast<TypeInteger>(element_expression))
        {
            Error::ShowError(Helper::FormatString("The number of elements in the array must be an integer! (receive '%s')",element_expression->ToString().c_str()), array->number_of_elements->location);
            return false;
        }
    }
    
    // If the variable is a class and we don't have an initializing value,  we need o theck if it correctly calls the constructor
    else if (VariableIsClass())
    {
        return CheckConstructor();
    }

    else
    {
        if (arguments_to_constructor.size() > 0)
        {
            Error::ShowError("Cannot pass arguments to a non-class variable!", this->location);
            return false;
        }
    }
    return true;
}

std::shared_ptr<VariableSymbol> VariableDefinitionNode::GetVariable()
{
    return variable;
}

std::shared_ptr<IRStatement> VariableDefinitionNode::TranslateToIR()
{

    auto varType = variable->GetType();

    auto varLocation = variable->TranslateExpressionToIr()->ToValueExpression();

    // If there is an expression, then the resource is already created so we just need to move it into this variable's location

    if(expression)
    {
        auto initializingValue = expression->TranslateExpressionToIr()->ToValueExpression();

        auto moveValueIntoVariable = std::make_shared<IRMove>(
            varLocation,
            initializingValue
        );

        return moveValueIntoVariable;
    }


    // If the variable is a class type then we need to allocate enough space on the heap for it, move the pointer to the allocated space to this variable's location and call the class's constructor with the location of this variable (so it will know where the variables of the correct object is)

    auto classType = std::dynamic_pointer_cast<ClassSymbol>(
        varType
    );

    if(classType)
    {
        std::vector<std::shared_ptr<IRStatement>> statements;

        // initClass function will be an external fucntion which will return a pointer to the heap-allocated space (but wont't initialize the spaces to 0)

        auto labelForClassInit = std::make_shared<Label>(
            "initClass"
        );
        
        // We need to pass the required space as an argument to the function

        auto expressionListForClassInit = std::make_shared<IRExpressionList>();

        expressionListForClassInit->expression = std::make_shared<IRConst>(
            classType->size_in_bytes
        );

        //FIXME: Pass the parameter to the function, moving the argument might need to be done by the code emitter
        auto moveSizeIntoRDI = std::make_shared<IRMove>(
            std::make_shared<IRTemp>(ReservedIrRegisters::RDI),
            std::make_shared<IRConst>(
                classType->size_in_bytes
            )
        );

        statements.push_back(moveSizeIntoRDI);

        auto argumentLocation = std::make_shared<IRExpressionList>();

        argumentLocation->expression = std::make_shared<IRTemp>(
            ReservedIrRegisters::RDI //FIXME: Set that the first argument can be found inside RDI maybe the argument location should be allcoated during canonical
        );

        argumentLocation->next = nullptr;
        
        auto callInitClass= std::make_shared<IRCall>(
            std::make_shared<IRName>(
                labelForClassInit
            ),
            expressionListForClassInit,
            argumentLocation,
            true // The return value is needed
        );

        // Move the return value of the function to this variable's location

        auto evaluateAndGetRAX = std::make_shared<IREseq>(
            std::make_shared<IRTemp>(ReservedIrRegisters::RAX),
            std::make_shared<IREvaluateExpression>(callInitClass)
        );

        auto moveToLocation = std::make_shared<IRMove>(
            varLocation,
            callInitClass
        );

        statements.push_back(moveToLocation);

        // We need to call the wanted constructor (which was saved by the semantic analyzer, if needed) with the allcoated space

        
        if (constructor)
        {
            // The first argument must be the location of the object

            auto argumentsForConstructor = TranslateArgumentsToConstructorToIR();


            auto sp = std::make_shared<IRTemp>(ReservedIrRegisters::StackPointer);

            constructor->TranslateToIRWithGivenParemeter(
                variable->access->AsExpressionNoDereference(sp), // The variable should be offseted from RSP, but shouldn't be dereferenced
                argumentsForConstructor,
                statements
            );
            
            

        }

        return std::make_shared<IRSequence>(
            statements
        );
    }





    // If the variable is an array type then we need to allocate space for it on the heap which will be done by an external function which will return a pointer to the start of the  allocated space

    auto arrayType = std::dynamic_pointer_cast<Array>(varType);

    // If there is an expression it means that we point this vairable to another array, so we don't have to allocate space for this one
    
    if(arrayType)
    {
        int sizeOfElement = arrayType->inner_type->GetSize();

        auto numberOfElementsExpression = arrayType->number_of_elements->TranslateExpressionToIr()->ToValueExpression();

        // Algorithm for calculating the space needed for an array: ((length_of_the_array) × (size_of_elements)) + 4
        // + 4 is needed because the first element stored will be the size of the array which is an integer

        // ((length_of_the_array)) × (size_of_elements)
        auto spaceNeededExpression = std::make_shared<IRBinaryOperator>(
            BinaryOperator::MULTIPLY,
            numberOfElementsExpression,
            std::make_shared<IRConst>(sizeOfElement)
        );

        // + 4 expression

        auto adjustedSpace = std::make_shared<IRBinaryOperator>(
            BinaryOperator::PLUS,
            spaceNeededExpression,
            std::make_shared<IRConst>(4)
        );

        auto labelForInitArray = std::make_shared<Label>("initArray");

        auto expressionListForFunctionCall = std::make_shared<IRExpressionList>();

        // Pass the space needed as a argument to the initArray function, so it knows how much space it should allocate

        expressionListForFunctionCall->expression = adjustedSpace;

        auto argumentLocation = std::make_shared<IRExpressionList>();

        argumentLocation->expression = std::make_shared<IRTemp>(
            ReservedIrRegisters::RDI //FIXME: Set that the first argument can be found inside RDI maybe the argument location should be allcoated during canonical
        );

        argumentLocation->next = nullptr;

        auto externalFunctionCall = std::make_shared<IRCall>(
            std::make_shared<IRName>(labelForInitArray),
            expressionListForFunctionCall,
            argumentLocation,
            true // Return value is needed
        );

        

        // Call the external function and move it's return value into this variable's location

        auto sp = std::make_shared<IRTemp>(ReservedIrRegisters::StackPointer);
        auto moveSpaceToLocation = std::make_shared<IRMove>(
            variable->access->AsExpressionNoDereference(sp), // The varaible location shouldn't be dereferenced
            externalFunctionCall
        );

        // Move the size of the array into the first element

        auto moveSizeIntoFirstElement = std::make_shared<IRMove>(
            varLocation,
            numberOfElementsExpression
        );

        auto allocateThenMoveSize = std::make_shared<IRSequence>(
            moveSpaceToLocation,
            moveSizeIntoFirstElement
        );

        return allocateThenMoveSize;
    }

    // We should move the value of the expression into the variable, if it exists, if there is no initializing value, the value of the varaible will be whatever there is in the memory

    

    //FIXME: Do something when there is no initializing value (maybe this shouldn't be allowed)

    return nullptr;
}

std::shared_ptr<IRExpressionList> VariableDefinitionNode::TranslateArgumentsToConstructorToIR()
{
    std::vector<std::shared_ptr<IRExpression>> expressions;

    for (auto &&argument : arguments_to_constructor)
    {
        expressions.push_back(
            argument->TranslateExpressionToIr()->ToValueExpression()
        );
    }
    
    return std::make_shared<IRExpressionList>(expressions);
}