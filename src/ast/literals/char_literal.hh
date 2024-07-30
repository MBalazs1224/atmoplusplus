#pragma once
#include "literal.hh"

class CharLiteral : public Literal
{
    char value;

    public:
        CharLiteral(char in) : value(in)
        {

        }
};