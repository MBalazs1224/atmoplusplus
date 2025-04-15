#include "ir_sequence.hh"

IRSequence::IRSequence(std::shared_ptr<IRStatement> l, std::shared_ptr<IRStatement> r) : left(std::move(l)),right(std::move(r))
{

}

IRSequence::IRSequence(const std::vector<std::shared_ptr<IRStatement>>& vec)
{
    assert(vec.size() >= 2);

    if (vec.size() == 2) {
        left = vec[0];
        right = vec[1];
    } else {
        
        left = vec[0];

        // Create a new vector excluding the first element
        std::vector<std::shared_ptr<IRStatement>> rest(vec.begin() + 1, vec.end());

        // Use recursion
        right = std::make_shared<IRSequence>(rest);
    }
    
}

std::string IRSequence::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    std::string dot = "node" + std::to_string(myId) + " [label=\"SEQ\"];\n";

    if (left)
    {
        dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
        dot += left->ToDotFormat(nodeCounter);
    }

    if (right)
    {
        dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
        dot += right->ToDotFormat(nodeCounter);
    }

    return dot;
}


std::shared_ptr<IRStatement> IRSequence::Build(std::shared_ptr<IRExpressionList> kids)
{
    throw std::logic_error("Build() not applicable to IRSequence");
        
}

std::shared_ptr<IRExpressionList> IRSequence::Kids()
{
    throw std::logic_error("Kids() not applicable to IRSequence");
}