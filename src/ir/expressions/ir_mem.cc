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