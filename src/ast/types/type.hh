#pragma once
#include <memory>
class Type
{
    public:
        virtual ~Type();
        Type()
        {

        }
        Type& operator=(const Type& other);

        // Function that will be used to check if the two types are the same
        virtual bool Equals(const std::shared_ptr<Type>) = 0;

        // Wrapper function for better readability
        virtual bool NotEquals(const std::shared_ptr<Type>);

        virtual std::string ToString() = 0;
};