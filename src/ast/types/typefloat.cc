#include "typefloat.hh"

bool TypeFloat::Compatible(const std::shared_ptr<Type> other)
{
    auto casted = std::dynamic_pointer_cast<TypeFloat>(other);
    return casted != nullptr;
}

std::string TypeFloat::ToString()
{
    return "type float";
}

DataSize TypeFloat::GetSize()
{
    // Float type is 8 bytes because it's always double precision
    return DataSize::QWord;
}
