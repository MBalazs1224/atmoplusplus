#pragma once
#include "literal.hh"

class CharLiteral : public Literal
{
    char value;

    public:
        CharLiteral(char in) : value(in)
        {

        }
        ~CharLiteral() = default;
    std::shared_ptr<Type> GetType() override
    {
        return std::make_shared<TypeChar>();
    }
    
    bool Check() override
    {
        // FIXME: Literals shouldn't be nodes
        return true;
    }
};