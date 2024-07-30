#pragma once
#include "literal.hh"

class FloatLiteral : public Literal
{
    double value;

    public:
        FloatLiteral(double in) : value(in)
        {

        }
};