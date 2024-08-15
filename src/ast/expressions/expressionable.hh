#pragma once
#include <memory>
#include "expressionable.hh"
#include "../../location/ilocation.hh"
class IExpressionable : public ILocation
{
    // FIXME: Not everything needs 2 expressions
    protected:
        std::shared_ptr<IExpressionable> left;
        std::shared_ptr<IExpressionable> right;
        IExpressionable(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in) : left(std::move(left_in)), right(std::move(right_in))
        {
            // FIXME: Thishas to be in an if statement because not exp only has the left_in and would segfault because right_in = nullptr
            if (left_in && right_in)
            {
                this->location = left_in->location + right_in->location;
            }
        }
        // Default function for expressions that doesn't use 2 epressions (literals)
        IExpressionable() : left(nullptr), right(nullptr)
        {

        }
    public:
        virtual ~IExpressionable() = 0;
        
};