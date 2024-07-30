#pragma once
#include "expressionable.hh"
#include <string>
#include <memory>
class Expression : public IExpressionable
{
    protected:
        std::unique_ptr<IExpressionable> left;
        std::unique_ptr<IExpressionable> right;
        
        Expression(std::unique_ptr<IExpressionable> left_in, std::unique_ptr<IExpressionable> right_in) : left(std::move(left_in)), right(std::move(right_in))
        {

        }
    /*
        If you delete an object through a pointer to the base class and the destructor is not virtual, only the base class destructor will be called. This can lead to resource leaks or undefined behavior because the derived class destructor will not be called, and the resources managed by the derived class will not be properly released.
    */
   // Public because unique_ptr has to call it to correctly delete itself
   public:
     virtual ~Expression();
     virtual std::string Type() = 0;
};