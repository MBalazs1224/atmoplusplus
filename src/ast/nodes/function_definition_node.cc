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

std::shared_ptr<IRStatement> FunctionDefinitionNode::TranslateToIR()
{
    //TODO: Implement that not all variables are the same size

    x86Frame current_frame;

    // Allocate space for the local variables
    for (auto &&var : function->local_variables)
    {
        // true means that the variable escapes, meaning it should be in frame not in register
        var->access = current_frame.allocLocal(true);
        

        // Give the variable the frame pointer
        var->frame_pointer = current_frame.FramePointer();
    }
    


    auto function_label = std::make_shared<Label>(function->name);

    std::vector<std::shared_ptr<IRStatement>> statements;

    // Create a label for the function
    statements.push_back(std::make_shared<IRLabel>(function_label));

    // Execute the body of the function
    statements.push_back(function->body->TranslateToIR());

    return std::make_shared<IRSequence>(statements);
}
