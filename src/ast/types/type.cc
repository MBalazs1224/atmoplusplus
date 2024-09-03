#include "type.hh"

Type::~Type() = default;

Type::Type(const Type& other)
{
    this->isArray = other.isArray;
}

Type& Type::operator=(const Type& other)
{
    // Guard self assignment
    if (this == &other)
        return *this;

    this->isArray = other.isArray;

    return *this;
}


bool Type::NotEquals(const std::shared_ptr<Type> other)
{
    return !this->Equals(other);
}