#include "char_literal.hh"

CharLiteral::CharLiteral(char in) : value(in)
{
}

std::shared_ptr<Type> CharLiteral::GetType()
{
    return std::make_shared<TypeChar>();
}

bool CharLiteral::Check()
{
    return true;
}
