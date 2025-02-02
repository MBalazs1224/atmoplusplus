#include "ir_expression_list.hh"

std::string IRExpressionList::ToDotFormat(int& nodeCounter)
{
    if (expression)
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