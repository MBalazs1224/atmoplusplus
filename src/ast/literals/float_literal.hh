#pragma once
#include "literal.hh"
class FloatLiteral : public Literal
{
    public:
        double value;

        FloatLiteral(double in);

        ~FloatLiteral() = default;

        std::shared_ptr<Type> GetType() override;  

        bool Check() override;

        std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;
};