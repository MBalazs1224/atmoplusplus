#pragma once
#include "attribute.hh"
class AttributeStatic : public Attribute
{
    public:     
    ~AttributeStatic() override = default;
    std::string ToString() override;

};