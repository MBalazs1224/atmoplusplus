#pragma once
#include "literal.hh"
class CharLiteral : public Literal
{
    public:
        char value;

        CharLiteral(char in);

        ~CharLiteral() = default;

        std::shared_ptr<Type> GetType() override;
    
        bool Check() override;

         std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;
};