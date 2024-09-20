#include "scope.hh"
#include <iostream>
#include <sstream>

void Scope::AddElement(const std::string& id, std::shared_ptr<SymbolTableElement> element)
{
    if (SymbolAlreadyDeclared(id))
    {
        Error::ShowError(Helper::FormatString("Multiple definition for symbol '%s'",id.c_str()),element->location);
    }
    else
    {
        elements[id] = element;
    }
}

bool Scope::SymbolAlreadyDeclared(const std::string& id)
{
    return elements.find(id) != elements.end();
}

std::shared_ptr<SymbolTableElement> Scope::GetElement(const std::string& id)
{
    if (!SymbolAlreadyDeclared(id))
    {
        if (parent)
        {
            return parent->GetElement(id);
        }
        return nullptr;
    }
    return elements[id];
}