#pragma once
#include "type.hh"

class TypeChar : public Type
{
    bool Compatible(const std::shared_ptr<Type> other) override
    {
        auto casted = std::dynamic_pointer_cast<TypeChar>(other);
        return casted != nullptr;
    }

    std::string ToString() override
    {
        return "type char";
    }
};