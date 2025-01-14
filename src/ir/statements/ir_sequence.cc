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