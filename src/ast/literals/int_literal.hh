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
    std::shared_ptr<Type> GetType() override
    {
        return std::make_shared<TypeInteger>();
    }    
    bool Check() override
    {
        // FIXME: Literals shouldn't be nodes
        return true;
    }
};