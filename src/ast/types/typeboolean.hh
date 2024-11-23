#pragma once
#include "type.hh"
#include <string>

class TypeBoolean : public Type
{
    bool Compatible(const std::shared_ptr<Type> other) override;
    std::string ToString() override;

    int GetSize() override;
};