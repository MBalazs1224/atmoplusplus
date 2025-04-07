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
    auto label = GlobalStrings::AddToPool(this->value);

    // Create an IRName that points to the created label
    auto exp = std::make_shared<IRName>(label);

    return std::make_shared<TranslateValueExpression>(exp);
}