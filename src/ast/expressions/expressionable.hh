#pragma once
#include <memory>
#include "expressionable.hh"
#include "../nodes/node.hh"
class IExpressionable : public Node
{
    // FIXME: Not everything needs 2 expressions
    protected:
        std::shared_ptr<IExpressionable> left;
        std::shared_ptr<IExpressionable> right;
        IExpressionable(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in, yy::location loc) : Node(loc), left(std::move(left_in)), right(std::move(right_in))
        {

        }
        // Default function for expressions that doesn't use 2 epressions (literals)
        IExpressionable() : Node(), left(nullptr), right(nullptr)
        {

        }
        IExpressionable(yy::location loc) : IExpressionable(nullptr,nullptr,loc)
        {
        }

    public:
        virtual ~IExpressionable() = 0;
        
};