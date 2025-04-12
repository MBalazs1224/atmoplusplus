#include "ir_name.hh"

IRName::IRName(std::shared_ptr<Label> l) : label(std::move(l))
{

}

std::string IRName::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    return "node" + std::to_string(myId) + " [label=\"NAME: " + label->ToString().c_str() + "\"];\n";
}

std::shared_ptr<IRExpression> IRName::Build(std::shared_ptr<IRExpressionList> kids)
{
    return shared_from_this();
}

std::shared_ptr<IRExpressionList> IRName::Kids()
{
    return nullptr;
}