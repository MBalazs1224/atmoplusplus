#pragma once
#include "type.hh"
#include <string>
class TypeFloat : public Type
{
    bool Compatible(const std::shared_ptr<Type> other) override;

    std::string ToString() override;
};