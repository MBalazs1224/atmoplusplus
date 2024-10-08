#pragma once
#include "type.hh"

class TypeBoolean : public Type
{
    bool Compatible(const std::shared_ptr<Type> other) override
    {
        auto casted = std::dynamic_pointer_cast<TypeBoolean>(other);
        return casted != nullptr;
    }
    std::string ToString() override
    {
        return "type boolean";
    }
};