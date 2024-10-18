#pragma once
#include "literal.hh"

class IntegerLiteral : public Literal
{

    public:
        IntegerLiteral(int in);
        ~IntegerLiteral() = default;
    std::shared_ptr<Type> GetType() override;   
    bool Check() override;
    int value;

};