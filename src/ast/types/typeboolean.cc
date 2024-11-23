#include "typeboolean.hh"

bool TypeBoolean::Compatible(const std::shared_ptr<Type> other)
{
    auto casted = std::dynamic_pointer_cast<TypeBoolean>(other);
    return casted != nullptr;
}

std::string TypeBoolean::ToString()
{
    return "type boolean";
}

DataSize TypeBoolean::GetSize()
{
    return DataSize::Byte;
}
