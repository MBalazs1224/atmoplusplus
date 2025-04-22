#include "ir_mem.hh"

IRMem::IRMem(std::shared_ptr<IRExpression> e, int bytes) : exp(std::move(e)), bytesNeeded(bytes)
{

}

std::string IRMem::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    std::string dot = "node" + std::to_string(myId) + " [label=\"MEM\"];\n";
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
    dot += exp->ToDotFormat(nodeCounter);
    return dot;
}

std::shared_ptr<IRExpression> IRMem::Build(std::shared_ptr<IRExpressionList> kids)
{
    return std::make_shared<IRMem>(
        kids->expression,
        kids->expression->Size()
    );
}

std::shared_ptr<IRExpressionList> IRMem::Kids()
{
    return std::make_shared<IRExpressionList>(
        this->exp,
        nullptr
    );
}

DataSize IRMem::Size()
{
    return (DataSize)this->bytesNeeded;
}