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
        virtual bool Compatible(const std::shared_ptr<Type>) = 0;

        // Wrapper function for better readability
        virtual bool NotCompatible(const std::shared_ptr<Type>);

        virtual std::string ToString() = 0;

        /*
        * @returns The size of the type in bytes.
        */
        virtual int GetSize() = 0;
};