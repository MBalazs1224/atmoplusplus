#pragma once
#include "type.hh"
#include <string>
class TypeString : public Type
{
    bool Compatible(const std::shared_ptr<Type> other) override;

    std::string ToString() override;

    
};