#include "ir_sequence.hh"

IRSequence::IRSequence(std::shared_ptr<IRStatement> l, std::shared_ptr<IRStatement> r) : left(std::move(l)),right(std::move(r))
{

}

IRSequence::IRSequence(const std::vector<std::shared_ptr<IRStatement>>& vec)
{
    // There should be at least one statement inside the vector
    assert(!vec.empty());

    // If we only have one statement we can only set the left expression
    if(vec.size() == 1)
    {
        left = vec[0];
        right = nullptr;
    }
    else
    {
        left = vec[0];
        // Copy the vector into a new vector which doesn't contain the first element
        std::vector<std::shared_ptr<IRStatement>> restOfStatements(vec.begin() + 1, vec.end());

        // Use recursion to assign the value of the rest of the vector to the right side
        right = std::make_shared<IRSequence>(restOfStatements);
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