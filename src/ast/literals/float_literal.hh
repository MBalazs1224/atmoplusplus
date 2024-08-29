#pragma once
#include "literal.hh"

class FloatLiteral : public Literal
{
    double value;

    public:
        FloatLiteral(double in) : value(in)
        {

        }
        ~FloatLiteral() = default;
    std::shared_ptr<Type> GetType() override
    {
        return std::make_shared<TypeFloat>();
    }
};