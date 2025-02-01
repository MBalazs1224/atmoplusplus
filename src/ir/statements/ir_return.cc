#include "ir_return.hh"

std::string IRReturn::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    
    return "node" + std::to_string(myId) + " [label=\"RETURN\"];\n";
}