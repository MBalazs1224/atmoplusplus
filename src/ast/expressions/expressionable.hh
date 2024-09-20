#pragma once
#include <memory>
#include "../nodes/node.hh"
#include "../types/all_types.hh"
// For showing eroor messages 
#include "../../helper/helper.hh"
#include "../../error/error.hh"
class IExpressionable : public Node
{
    protected:
        IExpressionable() : Node()
        {

        }
        IExpressionable(yy::location loc) : Node(loc)
        {
        }

    public:
        virtual ~IExpressionable() = 0;
        virtual std::shared_ptr<Type> GetType() = 0;
        
};
