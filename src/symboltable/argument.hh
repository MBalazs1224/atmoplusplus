#pragma once
#include <string>
#include "../ast/types/type.hh"
#include <memory>
class Argument
{
    public:
        std::string name;
        std::unique_ptr<Type> type;
        Argument(std::string name_in,std::unique_ptr<Type> type_in) : name(name_in), type(std::move(type_in))
        {

        }
}; 