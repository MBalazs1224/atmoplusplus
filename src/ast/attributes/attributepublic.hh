#pragma once
#include "attribute.hh"
class AttributePublic : public Attribute
{
    public:
    ~AttributePublic() override = default;
    std::string ToString() override;
};