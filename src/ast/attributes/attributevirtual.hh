#pragma once
#include "attribute.hh"

class AttributeVirtual : public Attribute
{
    public:
    ~AttributeVirtual() override = default;
    std::string ToString() override;
};