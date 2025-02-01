#include "ir_move.hh"

IRMove::IRMove(std::shared_ptr<IRExpression> src_in, std::shared_ptr<IRExpression> dest_in) : source(std::move(src_in)), destination(std::move(dest_in))
{

}

std::string IRMove::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;

    std::string dot = "node" + std::to_string(myId) + " [label=\"MOVE\"];\n";

    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";

    dot += destination->ToDotFormat(nodeCounter);

    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";

    dot += source->ToDotFormat(nodeCounter);

    return dot;
}