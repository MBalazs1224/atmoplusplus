#pragma once
#include "literal.hh"

class FloatLiteral : public Literal
{

    public:
        FloatLiteral(double in);
        ~FloatLiteral() = default;
    std::shared_ptr<Type> GetType() override;   
    bool Check() override;
    double value;

};