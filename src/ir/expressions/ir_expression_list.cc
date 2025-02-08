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

    // Recursively build the linked list
    std::shared_ptr<IRExpressionList> current = std::make_shared<IRExpressionList>();
    this->next = current;

    for (size_t i = 1; i < expressions.size(); i++)
    {
        current->expression = expressions[i];

        if (i + 1 < expressions.size()) // Create a new node if more elements exist
        {
            current->next = std::make_shared<IRExpressionList>();
            current = current->next;
        }
        else
        {
            current->next = nullptr; // Mark end of list
        }
    }
}