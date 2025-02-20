#pragma once
#include "attribute.hh"

class AttributeOverriding : public Attribute
{
    public:
    ~AttributeOverriding() override = default;
    std::string ToString() override;
};