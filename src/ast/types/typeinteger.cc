#include "typeinteger.hh"

bool TypeInteger::Compatible(const std::shared_ptr<Type> other)
{
    auto casted = std::dynamic_pointer_cast<TypeInteger>(other);
    return casted != nullptr;
}

std::string TypeInteger::ToString()
{
    return "type integer";
}

int TypeInteger::GetSize()
{
    // Integer type is 4 bytes
    return 4;
}
