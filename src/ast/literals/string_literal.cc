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

    auto pair_ = std::make_pair(label,value);

   // TODO: Strings need to be added to a list of all global strings, so that they can be emitted by the code generator into the data section of the assembly file

    // Create an IRName that points to the created label
    auto exp = std::make_shared<IRName>(label);

    return std::make_shared<TranslateValueExpression>(exp);
}