#include "integer_literal.hh"
IntegerLiteral::IntegerLiteral(int in) : value(in)
{
}

std::shared_ptr<Type> IntegerLiteral::GetType()
{
    return std::make_shared<TypeInteger>();
}

bool IntegerLiteral::Check()
{
    return true;
}

std::shared_ptr<TranslateExpression> IntegerLiteral::TranslateExpressionToIr()
{
    //TODO: "Implement integer literal to ir"
    return nullptr;
}