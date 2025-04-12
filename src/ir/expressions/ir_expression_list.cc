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

IRExpressionList::IRExpressionList(std::shared_ptr<IRExpression> head, std::shared_ptr<IRExpressionList> nextIn)
{
    this->expression = head;
    this->next = nextIn;
}

std::shared_ptr<IRExpressionList> IRExpressionList::CreateFromVector(std::vector<std::shared_ptr<IRExpression>>& expressions)
{
    // If the vector is empty, return nullptr
    if (expressions.empty())
    {
        return nullptr;
    }

    // Create the head of the linked list
    auto head = std::make_shared<IRExpressionList>();
    head->expression = expressions[0];

    // Pointer to the last added node
    auto current = head;

    // Build the linked list iteratively
    for (size_t i = 1; i < expressions.size(); i++)
    {
        current->next = std::make_shared<IRExpressionList>();
        current = current->next;
        current->expression = expressions[i];
    }

    // Ensure the last node's next pointer is nullptr
    current->next = nullptr;

    // Return the head of the linked list
    return head;
}