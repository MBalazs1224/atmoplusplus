#include "typestring.hh"

bool TypeString::Compatible(const std::shared_ptr<Type> other)
{
    auto casted = std::dynamic_pointer_cast<TypeString>(other);
    return casted != nullptr;
}

std::string TypeString::ToString()
{
    return "type string";
}
