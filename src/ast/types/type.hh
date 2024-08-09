#pragma once
class Type
{
    protected:
        bool isArray = false;
    public:
        virtual ~Type() = default;
        void SetIsArray(bool isArray)
        {
            this->isArray = isArray;
        }
        Type()
        {
            
        }
        Type(const Type& other)
        {
            this->isArray = other.isArray;
        }
};