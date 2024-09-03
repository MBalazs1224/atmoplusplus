#include "scope.hh"
#include <iostream>
#include <sstream>
void Scope::AddChild(std::shared_ptr<Scope> child)
{
    children.push_back(child);
}

void Scope::AddElement(const std::string& id, std::shared_ptr<SymbolTableElement> element)
{
    if (SymbolAlreadyDeclared(id))
    {
        //FIXME: Stringstream on error mesages looks unnecessary
        std::stringstream s;
        s << "Multiple definition of symbol '" << id << "'!";
        Error::ShowError(s.str(),element->location);
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