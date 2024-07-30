#pragma once
#include "literal.hh"

class IntegerLiteral : public Literal
{
    int value;

    public:
        IntegerLiteral(int in) : value(in)
        {

        }
        ~IntegerLiteral() = default;
};