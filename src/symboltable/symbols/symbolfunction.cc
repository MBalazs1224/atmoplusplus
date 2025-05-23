#include "symbolfunction.hh"

FunctionSymbol::FunctionSymbol(std::shared_ptr<Type> type_in, std::vector<std::shared_ptr<Attribute>> attr_in, std::vector<std::shared_ptr<VariableSymbol>> args_in, std::unique_ptr<BodyNode> body_in, std::string name_in)
    : SymbolTableElement(type_in, std::move(attr_in)), arguments(std::move(args_in)),body(std::move(body_in))
    {
        this->name = name_in;
        // By defaule the generated name in assembly should be the function's name
        this->nameInAssembly = name;

        // All functions (by default) can be accessed by a label of it's name 
        this->access = std::make_shared<PrintedLabel>(
            std::make_shared<Label>(this->nameInAssembly)
        );
    }

FunctionSymbol::FunctionSymbol(std::vector<std::shared_ptr<Attribute>> attr_in, std::unique_ptr<BodyNode> body_in, std::vector<std::shared_ptr<VariableSymbol>> args_in) : FunctionSymbol(nullptr, attr_in, args_in,std::move(body_in), "") {}

FunctionSymbol::FunctionSymbol(std::unique_ptr<BodyNode> body_in) : FunctionSymbol(
    std::vector<std::shared_ptr<Attribute>>(),
    std::move(body_in),
    std::vector<std::shared_ptr<VariableSymbol>>()
) {}

std::shared_ptr<Type> FunctionSymbol::GetType() {
    if (auto id = std::dynamic_pointer_cast<Identifier>(type)) {
        return id->GetType();
    }
    return this->type;
}

bool FunctionSymbol::CheckAttributes()
{
    int numberOfAccessModifiers = 0;

    bool returnValue = true;

    bool hasDuplicates = std::adjacent_find(attributes.begin(),attributes.end()) != attributes.end();

    if(hasDuplicates)
    {
        Error::ShowError("An attribute has been specified multiple times!", this->location);
        returnValue = false;
    }

    auto numberOfPolymorphicAttributes = 0;

    for (auto &&attribute : attributes)
    {
        if(std::dynamic_pointer_cast<AttributeVirtual>(attribute))
        {
            numberOfPolymorphicAttributes++;
            this->isVirtual = true;
            if(numberOfPolymorphicAttributes > 1)
            {
                Error::ShowError("A function can only have maximum 1 polyphormic modifier!", this->location);
                returnValue = false;
            }
        }
        else if(std::dynamic_pointer_cast<AttributeOverriding>(attribute))
        {
            numberOfPolymorphicAttributes++;;
            this->isOverriding = true;
            if(numberOfPolymorphicAttributes > 1)
            {
                Error::ShowError("A function can only have maximum 1 polyphormic modifier!", this->location);
                returnValue = false;
            }
        }
        // A function cannot have multiple access modifiers like public and private at the same time
        if(std::dynamic_pointer_cast<AttributePublic>(attribute) || std::dynamic_pointer_cast<AttributePrivate>(attribute) ||std::dynamic_pointer_cast<AttributeProtected>(attribute))
        {
            numberOfAccessModifiers++;
            if(numberOfAccessModifiers > 1)
            {
                Error::ShowError("A function can only have maximum 1 access modifier!",this->location);
                returnValue = false;
            }
        }
        
    }

    // If the function doesn't have an access modifier, then default to private
    if(numberOfAccessModifiers == 0)
    {
        attributes.push_back(
            std::make_shared<AttributePrivate>()
        );
    }

    return returnValue;
    
}

bool FunctionSymbol::Check() {
    if(arguments.size() > 6)
    {
        Error::ShowError("Functions cannot have  more than 6 arguments!",this->location);
        return false;
    }

    if(!CheckAttributes())
    {
        return false;
    }

    if(!body->Check())
    {
        return false;
    }

    // Get all the variables
    local_variables = body->GetVariables();

    // Calculate the size of the function
    for (auto &&variable : local_variables)
    {
        this->size_in_bytes += variable->GetSize();
    }
    // If nothing set some other assemby name for this function (class, etc) then it's assembly label should be it's user given name
    if(this->nameInAssembly == "")
    {
        this->nameInAssembly = this->name;
    }

    auto returnNodes = this->body->GetReturnNodes();

    for (auto &&returnNode : returnNodes)
    {
        // Set that the return nodes should also pop the callee-saved registers before returning
        returnNode->shouldPopRegisters = true;
    }
    
    

    return true;
}

const std::vector<std::shared_ptr<VariableSymbol>>& FunctionSymbol::GetArguments() {
    return arguments;
}

// If we want to create an empty function (for destructors, constructors etc.) we need it to have a body but with an empty vector of statements to prevent segfaults
                                    // Implicitly converts the {std::make_shared<AttributePublic>()} to a vector
FunctionSymbol::FunctionSymbol() : SymbolTableElement({std::make_shared<AttributePublic>()}), arguments() , body(std::make_unique<BodyNode>(
    std::vector<std::shared_ptr<Node>>()
)) 
{

}


DataSize FunctionSymbol::GetSize()
{
    // Functions don't have a size
    return DataSize::Empty;
}

std::shared_ptr<TranslateExpression> FunctionSymbol::TranslateExpressionToIr()
{
    // If a function is accessed trough this function (not a member access) then it must be at a label, so the offset can be ignored

    auto accessExp = this->access->AsExpression(nullptr);

    return std::make_shared<TranslateValueExpression>(accessExp);
}



std::shared_ptr<BoolList>  FunctionSymbol::GetWetherVariablesEscape()
{
    std::shared_ptr<BoolList> boolList = nullptr;

    for (size_t i = 0; i < this->local_variables.size(); i++)
    {
        // In this language all local variables should be in frame
        boolList = std::make_shared<BoolList>(true, boolList);
        boolList->sizeOfVariable = local_variables[i]->GetSize();
    }

    return boolList;
}

bool FunctionSymbol::IsInClass()
{
    return containingClass != nullptr;
}

std::shared_ptr<IRExpressionList> FunctionSymbol::ConvertParameterLocationToList()
{

    //FIXME: Arguments should be called parameters!!!!!!!!!!!!!

    // If the vector is empty, return nullptr
    if (this->arguments.empty())
    {
        return nullptr;
    }

    // Create the head of the linked list
    std::shared_ptr<IRExpressionList> head = std::make_shared<IRExpressionList>();
    std::shared_ptr<IRExpressionList> current = head;

    // Iterate through the vector
    for (size_t i = 0; i < this->arguments.size(); ++i)
    {
        // Set the current node's expression
        current->expression = arguments[i]->TranslateExpressionToIr()->ToValueExpression();

        // If this is not the last element, create a new node for the next element
        if (i < this->arguments.size() - 1)
        {
            current->next = std::make_shared<IRExpressionList>();
            current = current->next;
        }
    }

    // Return the head of the linked list
    return head;
}


/*
    1. pseudo-instructions, as needed in the particular assembly language, to an-
    nounce the beginning of a function;
    2. a label definition for the function name;
    3. an instruction to adjust the stack pointer (to allocate a new frame);
    4. instructions to save “escaping” arguments into the frame, and to move nonescap-
    ing arguments into fresh temporary registers;
    5. store instructions to save any callee-save registers – including the return ad-
    dress register – used within the function.
    Then comes
    6. the function body.
    The epilogue comes after the body and contains
    7. an instruction to move the return value (result of the function) to the register
    reserved for that purpose;
    8. load instructions to restore the callee-save registers;
    9. an instruction to reset the stack pointer (to deallocate the frame);
    10. a return instruction ( JUMP to the return address);
    11. pseudo-instructions, as needed, to announce the end of a function.

    Andrew Appel - Modern Compiler Implementation in Java [p. ~156]
*/

std::shared_ptr<IRStatement> FunctionSymbol::TranslateToIR()
{
    // Set that if the function tries to access a variable inside the same class it was declared in, then it should offset it from RDI (which will be the this pointer)
    if(containingClass)
    {
        containingClass->VariablesShouldUseRDI(true);

        // If the function is inside a class, then it receives the this pointer as it's first argument, so we need to adjust the argument vector

        auto classTypeVar = std::make_shared<VariableSymbol>(
            containingClass, // The type of the variable is the type of the containing class (ClassSymbols act as types)
            std::vector<std::shared_ptr<Attribute>> {std::make_shared<AttributePrivate>()} // Attribute must be a vector containng private (doesn't really matter, because it's not inside a class but just in case)
        );

        // Insert the this pointer to the start of the vector
        arguments.insert(
            arguments.begin(),
            classTypeVar
        );
    }

    
    // The frame element that will hold the information

    auto escapesForVariables = GetWetherVariablesEscape();


    auto currentFrame = GlobalFrame::globalFrameType->newFrame(this->nameInAssembly,escapesForVariables);

    // Set the variables access to the generated accessList (the list is still in reverse)

    int indexOfLocalVar = this->local_variables.size() - 1;

    std::shared_ptr<AccessList> currentAccess = currentFrame->formals;

    while (currentAccess != nullptr)
    {
        this->local_variables[indexOfLocalVar]->access = currentAccess->head;

        //Get the next access object
        currentAccess = currentAccess->tail;

        //Get the previous var
        indexOfLocalVar--;
    }


    

    currentFrame->AllocateRegisters(this->arguments);

    // If there is no body for the function (like empty constructors or destructors) we just define a dummy body which will just return immediately (maybe we should just ignore these types of function? FIXME)

    if(body->isEmpty())
    {
        auto label = std::make_shared<Label>(this->nameInAssembly);
        auto retIns = std::make_shared<IRReturn>();

        auto seq = std::make_shared<IRSequence>(
            std::make_shared<IRLabel>(label),
            retIns
        );

        if(containingClass)
         {
              // Reset RDI offset even if the body is empty!
              containingClass->VariablesShouldUseRDI(false);
         }

        return seq;
        
    }
    
    // (1-2-3-6-9-10-11) Use the frame's function to do the neccessary instructions
    auto bodyInstructions = this->body->TranslateToIR();

    // (4-5-8) Use the frames function to save registers and return the correct value

    auto newBody = currentFrame->ProcessFunctionEntryAndExit1(bodyInstructions);
    // (7) Move the return value to the correct regsiter

    // TODO: Implement moving the return value
    auto adjustedBody = currentFrame->ProcessFunctionEntryAndExit3(this->nameInAssembly,newBody);

    if(containingClass)
    {
        containingClass->VariablesShouldUseRDI(false);
    }


    return adjustedBody;
}