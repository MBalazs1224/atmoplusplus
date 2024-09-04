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
        ~StringLiteral() = default;
    std::shared_ptr<Type> GetType() override
    {
        return std::make_shared<TypeString>();
    }    
    void Check() override
    {
        // FIXME: Literals shouldn't be nodes
    }
};