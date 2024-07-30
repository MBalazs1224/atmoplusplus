#pragma once
#include "literal.hh"
#include <string>
class StringLiteral : public Literal
{
    std::string value;

    public:
        StringLiteral(std::string in) : value(in)
        {

        }
};