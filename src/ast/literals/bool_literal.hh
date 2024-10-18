#pragma once
#include "literal.hh"

class BooleanLiteral : public Literal
{

    public:
        BooleanLiteral(bool in);
        ~BooleanLiteral() = default;

    std::shared_ptr<Type> GetType() override;
    bool Check() override;
    bool value;

};