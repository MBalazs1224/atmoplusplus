#pragma once
#include "type.hh"
class TypeString : public Type
{
bool Equals(const std::shared_ptr<Type> other) override
    {
        auto casted = std::dynamic_pointer_cast<TypeString>(other);
        return casted != nullptr;
    }

    std::string ToString() override
    {
        return "type string";
    }

    
};