#pragma once
#include <string>
class Attribute
{
    public:
        virtual ~Attribute();
        virtual std::string ToString() = 0;
};