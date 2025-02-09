#include "ir_expression_list.hh"

std::string IRExpressionList::ToDotFormat(int& nodeCounter)
{
    if (!expression)
        return ""; // Empty list case
    int myId = nodeCounter++;
    std::string dot = "node" + std::to_string(myId) + " [label=\"ARG\"];\n";
    // Add head node
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";

    // There can be full empty expressionLists, like calling a function with no arguments
    if(expression)
    {
        dot += expression->ToDotFormat(nodeCounter);

    }
    // Recursively add the rest of the list
    if (next)
    {
        dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
        dot += next->ToDotFormat(nodeCounter);
    }
    return dot;
}

IRExpressionList::IRExpressionList(std::vector<std::shared_ptr<IRExpression>>& expressions)
{
    if (expressions.empty())
    {
        expression = nullptr;
        next = nullptr;
        return;
    }

    // Assign the first expression
    expression = expressions[0];

    // Pointer to the last added node
    std::shared_ptr<IRExpressionList>* current = &next;

    // Build the linked list iteratively
    for (size_t i = 1; i < expressions.size(); i++)
    {
        *current = std::make_shared<IRExpressionList>();
        (*current)->expression = expressions[i];
        current = &((*current)->next);
    }

    // Ensure last node correctly points to nullptr
    *current = nullptr;
}