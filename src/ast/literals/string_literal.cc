#include "string_literal.hh"
StringLiteral::StringLiteral(std::string in) : value(in)
{
}
std::shared_ptr<Type> StringLiteral::GetType()
{
    return std::make_shared<TypeString>();
}
bool StringLiteral::Check()
{
    return true;
}

std::shared_ptr<TranslateExpression> StringLiteral::TranslateExpressionToIr()
{
    //TODO: "Implement string literal to ir"
    return nullptr;
}