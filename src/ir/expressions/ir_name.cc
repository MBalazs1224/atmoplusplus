#include "ir_name.hh"

IRName::IRName(std::shared_ptr<Label> l) : label(std::move(l))
{

}

std::string IRName::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    return "node" + std::to_string(myId) + " [label=\"NAME: " + label->ToString() + "\"];\n";
}