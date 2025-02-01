#include "ir_label.hh"

IRLabel::IRLabel(std::shared_ptr<Label> l) : label(std::move(l))
{

}

std::string IRLabel::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    return "node" + std::to_string(myId) + " [label=\"IRLABEL: " + label->ToString() + "\"];\n";
}