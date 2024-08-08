#pragma once
#include <memory>
#include "expressionable.hh"
#include "../../location/ilocation.hh"
class IExpressionable : public ILocation
{
    protected:
        std::unique_ptr<IExpressionable> left;
        std::unique_ptr<IExpressionable> right;
        IExpressionable(std::unique_ptr<IExpressionable> left_in, std::unique_ptr<IExpressionable> right_in) : left(std::move(left_in)), right(std::move(right_in))
        {

        }
        // Default function for expressions that doesn't use 2 epressions (literals)
        IExpressionable() : left(nullptr), right(nullptr)
        {

        }
    public:
        virtual ~IExpressionable() = 0;
        
};