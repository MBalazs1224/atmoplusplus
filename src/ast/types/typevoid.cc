#include "typevoid.hh"

bool TypeVoid::Compatible(const std::shared_ptr<Type> other)
{
    auto casted = std::dynamic_pointer_cast<TypeVoid>(other);
    return casted != nullptr;
}

std::string TypeVoid::ToString()
{
    return "type void";
}

int TypeVoid::GetSize()
{
    // Void type has no size
    return 0;
}
