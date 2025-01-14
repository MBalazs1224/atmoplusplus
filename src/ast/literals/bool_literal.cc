#include "bool_literal.hh"
BooleanLiteral::BooleanLiteral(bool in) : value(in)
{
}

std::shared_ptr<Type> BooleanLiteral::GetType()
{
    return std::make_shared<TypeBoolean>();
}

bool BooleanLiteral::Check()
{
    return true;
}

std::shared_ptr<TranslateExpression> BooleanLiteral::TranslateExpressionToIr()
{
    int boolasint = value;

    auto exp = std::make_shared<IRConst>(boolasint);
    return std::make_shared<TranslateValueExpression>(exp);
}