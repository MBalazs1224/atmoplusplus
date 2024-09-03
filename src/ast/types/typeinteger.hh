#pragma once
#include "type.hh"

class TypeInteger : public Type
{
bool Equals(const std::shared_ptr<Type> other) override
    {
        auto casted = std::dynamic_pointer_cast<TypeInteger>(other);
        return casted && this->isArray == casted->isArray;
    }

    std::string ToString() override
    {
        return "type integer";
    }
};