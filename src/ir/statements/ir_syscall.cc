#include "ir_syscall.hh"

std::string IRSysCall::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    
    return "node" + std::to_string(myId) + " [label=\"SYSCALL\"];\n";
}

std::shared_ptr<IRStatement> IRSysCall::Build(std::shared_ptr<IRExpressionList> kids)
{
    return shared_from_this();
        
}

std::shared_ptr<IRExpressionList> IRSysCall::Kids()
{
    return nullptr;
}