#include "ir_syscall.hh"

std::string IRSysCall::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    
    return "node" + std::to_string(myId) + " [label=\"SYSCALL\"];\n";
}