#pragma once
#include "type.hh"

class TypeFloat : public Type
{
bool Equals(const std::shared_ptr<Type> other) override
    {
        auto casted = std::dynamic_pointer_cast<TypeFloat>(other);
        return casted && this->isArray == casted->isArray;
    }
};