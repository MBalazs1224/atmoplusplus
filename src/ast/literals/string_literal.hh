#pragma once
#include "literal.hh"
#include <string>
class StringLiteral : public Literal
{
    std::string value;

    public:
        StringLiteral(std::string in);
        ~StringLiteral() = default;
    std::shared_ptr<Type> GetType() override;  
    bool Check() override;
};