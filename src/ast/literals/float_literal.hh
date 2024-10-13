#pragma once
#include "literal.hh"

class FloatLiteral : public Literal
{
    double value;

    public:
        FloatLiteral(double in);
        ~FloatLiteral() = default;
    std::shared_ptr<Type> GetType();   
    bool Check() override;
};