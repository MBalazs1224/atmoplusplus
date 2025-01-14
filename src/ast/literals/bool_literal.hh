#pragma once
#include "literal.hh"
class BooleanLiteral : public Literal
{
    public:
        bool value;

        BooleanLiteral(bool in);

        ~BooleanLiteral() = default;

        std::shared_ptr<Type> GetType() override;
        
        bool Check() override;

        std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;

};