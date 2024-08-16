#pragma once
#include <string>
#include "../ast/types/type.hh"
#include <memory>
class Argument
{
    public:
        std::string name;
        Type type;
        Argument(std::string name_in,Type type_in) : name(name_in), type(type_in)
        {

        }
}; 