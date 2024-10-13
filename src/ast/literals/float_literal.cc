#include "float_literal.hh"

FloatLiteral::FloatLiteral(double in) : value(in)
{
}

std::shared_ptr<Type> FloatLiteral::GetType()
{
    return std::make_shared<TypeFloat>();
}

bool FloatLiteral::Check()
{
    return true;
}
