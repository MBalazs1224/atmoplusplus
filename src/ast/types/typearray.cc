#include "typearray.hh"



Array::Array(std::shared_ptr<Type> type_in)
{
    inner_type = std::move(type_in);
}

std::string Array::ToString()
{
    return Helper::FormatString("array of '%s'", inner_type->ToString().c_str());
}

bool Array::Compatible(const std::shared_ptr<Type> other)
{
    auto casted = std::dynamic_pointer_cast<Array>(other);
    return casted && this->inner_type->Compatible(casted->inner_type);
}