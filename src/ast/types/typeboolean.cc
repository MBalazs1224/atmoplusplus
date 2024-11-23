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

int TypeBoolean::GetSize()
{
    // Boolean type is 1 byte
    return 1;
}
