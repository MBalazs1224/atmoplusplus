#pragma once
#include "literal.hh"

class CharLiteral : public Literal
{
    char value;

    public:
        CharLiteral(char in);
        ~CharLiteral() = default;
    std::shared_ptr<Type> GetType() override;
    
    bool Check() override;
};