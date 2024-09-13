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

    std::shared_ptr<Type> GetType() override
    {
        return std::make_shared<TypeBoolean>();
    }
    bool Check() override
    {
        // FIXME: Literals shouldn't be nodes
        return true;
    }
};