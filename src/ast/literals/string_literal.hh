#pragma once
#include "literal.hh"
#include <string>
class StringLiteral : public Literal
{
    public:
        std::string value;

        StringLiteral(std::string in);

        ~StringLiteral() = default;

        std::shared_ptr<Type> GetType() override;  

        bool Check() override;

        std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;

};