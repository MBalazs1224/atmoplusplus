#include "identifier.hh"
#include "../../symboltable/symboltableelement.hh"
#include <utility>

Identifier::Identifier(std::shared_ptr<SymbolTableElement> element_in, std::string name_in, yy::location loc)
    : IExpressionable(loc), element(std::move(element_in)), name(std::move(name_in)) {}

void Identifier::SetElement(std::shared_ptr<SymbolTableElement> element)
{
    this->element = std::move(element);
}

std::shared_ptr<SymbolTableElement> Identifier::GetElement() {
    return element;
}

bool Identifier::Equals(const std::shared_ptr<Type> other)
{
    return other->Equals(element->GetType());
}

std::string Identifier::ToString()
{
    return element->GetType()->ToString();
}

bool Identifier::Check()
{
    if (!element) {
        Error::ShowError(Helper::FormatString("Unknown identifier '%s'!", name.c_str()), location);
    }
    return element != nullptr;
}

std::shared_ptr<Type> Identifier::GetType()
{
    assert(element);
    return element->GetType();
}
