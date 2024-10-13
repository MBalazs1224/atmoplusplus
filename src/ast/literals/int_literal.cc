#include "int_literal.hh"

IntegerLiteral::IntegerLiteral(int in) : value(in)
{
}

std::shared_ptr<Type> IntegerLiteral::GetType()
{
    return std::make_shared<TypeInteger>();
}

bool IntegerLiteral::Check()
{
    return true;
}
