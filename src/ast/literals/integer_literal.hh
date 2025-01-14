#pragma once
#include "literal.hh"
class IntegerLiteral : public Literal
{
    public:
        int value;

        IntegerLiteral(int in);

        ~IntegerLiteral() = default;

        std::shared_ptr<Type> GetType() override; 

        bool Check() override;

        std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;

};