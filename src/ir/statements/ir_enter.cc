#include "ir_enter.hh"

std::string IREnter::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    return "node" + std::to_string(myId) + " [label=\"ENTER: " + std::to_string(neededBytes) + "\"];\n";
}

IREnter::IREnter(int neededBytes)
{
    this->neededBytes = neededBytes;
}