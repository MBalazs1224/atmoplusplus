#include "ir_leave.hh"

std::string IRLeave::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    
    return "node" + std::to_string(myId) + " [label=\"LEAVE\"];\n";
}

std::shared_ptr<IRStatement> IRLeave::Build(std::shared_ptr<IRExpressionList> kids)
{
    return shared_from_this();
}

std::shared_ptr<IRExpressionList> IRLeave::Kids()
{
    return nullptr;
}