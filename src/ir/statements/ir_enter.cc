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

std::shared_ptr<IRStatement> IREnter::Build(std::shared_ptr<IRExpressionList> kids)
{
    return shared_from_this();
}

std::shared_ptr<IRExpressionList> IREnter::Kids()
{
    return nullptr;
}