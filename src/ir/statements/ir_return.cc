#include "ir_return.hh"

std::string IRReturn::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    
    return "node" + std::to_string(myId) + " [label=\"RETURN\"];\n";
}

std::shared_ptr<IRStatement> IRReturn::Build(std::shared_ptr<IRExpressionList> kids)
{
    return shared_from_this();
        
}

std::shared_ptr<IRExpressionList> IRReturn::Kids()
{
    return nullptr;
}