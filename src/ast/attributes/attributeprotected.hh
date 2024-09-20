#pragma once
#include "attribute.hh"
class AttributeProtected : public Attribute
{
    public:
    ~AttributeProtected() override = default;
    std::string ToString() override;
};