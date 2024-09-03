#pragma once
#include <memory>
class Type
{
    protected:
        bool isArray = false;
    public:
        virtual ~Type();
        void SetIsArray(bool isArray)
        {
            this->isArray = isArray;
        }
        Type()
        {

        }
        Type(const Type& other);
        Type& operator=(const Type& other);

        // Function that will be used to check if the two types are the same
        virtual bool Equals(const std::shared_ptr<Type>) = 0;
};