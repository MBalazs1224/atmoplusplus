#pragma once
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
};