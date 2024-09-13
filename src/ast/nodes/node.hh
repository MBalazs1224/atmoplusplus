#pragma once
#include "../../location/ilocation.hh"
class Node : public ILocation
{
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
        virtual bool Check() = 0;
};