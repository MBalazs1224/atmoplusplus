#pragma once
#include "attribute.hh"
class AttributePrivate : public Attribute
{
    public:
    ~AttributePrivate() override = default;
    std::string ToString() override;
};