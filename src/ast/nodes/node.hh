#pragma once
#include "../../location/ilocation.hh"

#include "../../ir/statements/ir_statement.hh"

#include <memory>

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

        virtual std::shared_ptr<IRStatement> TranslateToIR() = 0;
};