#include "type.hh"

Type::~Type() = default;


Type& Type::operator=(const Type& other)
{
    // Guard self assignment
    if (this == &other)
        return *this;


    return *this;
}


bool Type::NotCompatible(const std::shared_ptr<Type> other)
{
    return !this->Compatible(other);
}