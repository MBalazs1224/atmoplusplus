#pragma once
#include "../../location/ilocation.hh"
class Node : public ILocation
{
    //TODO: Add virtual destructor to ensure better cleanup
    protected:

    Node()
    {

    }
    Node(yy::location loc)
    {
        location = loc;
    }
    public:
        virtual ~Node() = default;
};