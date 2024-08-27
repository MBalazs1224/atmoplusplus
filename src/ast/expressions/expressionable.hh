#pragma once
#include <memory>
#include "expressionable.hh"
#include "../nodes/node.hh"
class IExpressionable : public Node
{
    // FIXME: Not everything needs 2 expressions
    // 
    protected:
               // Default function for expressions that doesn't use 2 epressions (literals)
        IExpressionable() : Node()
        {

        }
        IExpressionable(yy::location loc) : Node(loc)
        {
        }

    public:
        virtual ~IExpressionable() = 0;
        
};
