#include "typechar.hh"

bool TypeChar::Compatible(const std::shared_ptr<Type> other)
{
    auto casted = std::dynamic_pointer_cast<TypeChar>(other);
    return casted != nullptr;
}

std::string TypeChar::ToString()
{
    return "type char";
}

int TypeChar::GetSize()
{
    // Char type is 1 byte
    return 1;
}
