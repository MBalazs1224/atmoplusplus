#include "function_definition_node.hh"

FunctionDefinitionNode::FunctionDefinitionNode(std::shared_ptr<FunctionSymbol> func_in)
    : function(std::move(func_in))
{
}

FunctionDefinitionNode::~FunctionDefinitionNode() = default;

bool FunctionDefinitionNode::Check()
{
    return function->Check();
}

std::shared_ptr<FunctionSymbol> FunctionDefinitionNode::GetFunction()
{
    return function;
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

std::shared_ptr<IRStatement> FunctionDefinitionNode::TranslateToIR()
{

    // The frame element that will hold the information

    x86Frame frame;
    auto escapesForVariables = GetWetherVariablesEscape();

    //TODO: Make the newFrame function a static function

    auto currentFrame = frame.newFrame(function->name,escapesForVariables);

    // (1-2-3-6-9-10-11) Use the frame's function to do the neccessary instructions
    auto bodyInstructions = function->body->TranslateToIR();

    auto adjustedBody = currentFrame->ProcessFunctionEntryAndExit3(function->name,bodyInstructions);

    // (4-5-8) Use the frames function to save registers and return the correct value
    auto secondAdjustedBody = currentFrame->ProcessFunctionEntryAndExit1(adjustedBody);

    // (7) Move the return value to the correct regsiter

    // TODO: Implement moving the return value


    return secondAdjustedBody;


}

std::shared_ptr<BoolList>  FunctionDefinitionNode::GetWetherVariablesEscape()
{
    std::shared_ptr<BoolList> boolList = nullptr;

    for (size_t i = 0; i < function->local_variables.size(); i++)
    {
        // In this language non of the variables escape, so false will indicate it for all of them
        boolList = std::make_shared<BoolList>(false, boolList);
    }

    return boolList;
}
