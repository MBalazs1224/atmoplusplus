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
    // Create a new label that will point to the string
    auto label = std::make_shared<Label>();


    GlobalStrings::stringToLabel[value] = label;

    // Create an IRName that points to the created label
    auto exp = std::make_shared<IRName>(label);

    return std::make_shared<TranslateValueExpression>(exp);
}