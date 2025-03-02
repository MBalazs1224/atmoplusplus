#include "ir_leave.hh"

std::string IRLeave::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    
    return "node" + std::to_string(myId) + " [label=\"LEAVE\"];\n";
}