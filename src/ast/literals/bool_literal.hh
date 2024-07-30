#pragma once
#include "literal.hh"

class BooleanLiteral : public Literal
{
    bool value;

    public:
        BooleanLiteral(bool in) : value(in)
        {

        }
        ~BooleanLiteral() = default;
};