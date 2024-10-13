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
