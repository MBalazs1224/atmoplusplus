#include "ir_statement_list.hh"

IRStatementList::IRStatementList(std::shared_ptr<IRStatement> head_in, std::shared_ptr<IRStatementList> tail_in) : head(head_in), tail(tail_in)
{
}

std::string IRStatementList::ToDotFormat(int &nodeCounter)
{
    if (!head)
        return ""; // Empty list case

    int myId = nodeCounter++;
    std::string dot = "node" + std::to_string(myId) + " [label=\"STMT LIST\"];\n";

    // Add head node
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
    dot += head->ToDotFormat(nodeCounter);

    // Recursively add the rest of the list
    if (tail)
    {
        dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
        dot += tail->ToDotFormat(nodeCounter);
    }

    return dot;
}
