#include "ir_pop.hh"

IRPop::IRPop(std::shared_ptr<IRExpression> expression_in) : exp(expression_in)
{

}

std::string IRPop::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;
    std::string dot = "node" + std::to_string(myId) + " [label=\"POP\"];\n";

    // Connect pop target
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
    dot += exp->ToDotFormat(nodeCounter);

    return dot;
}