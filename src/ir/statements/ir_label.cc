#include "ir_label.hh"

IRLabel::IRLabel(std::shared_ptr<Label> l) : label(std::move(l))
{

}

std::string IRLabel::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    return "node" + std::to_string(myId) + " [label=\"IRLABEL: " + label->ToString().c_str() + "\"];\n";
}


std::shared_ptr<IRStatement> IRLabel::Build(std::shared_ptr<IRExpressionList> kids)
{
    return shared_from_this();
}

std::shared_ptr<IRExpressionList> IRLabel::Kids()
{
    return nullptr;
}