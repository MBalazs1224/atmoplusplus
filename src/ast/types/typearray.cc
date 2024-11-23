#include "typearray.hh"



Array::Array(std::shared_ptr<Type> type_in, std::shared_ptr<IExpressionable> number_of_elements_in) : inner_type(type_in), number_of_elements(number_of_elements_in)
{
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

int Array::GetSize()
{
    // The array will be a pointer so it will be 8 bytes
    return 8;
}