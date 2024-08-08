#include "scope.hh"
#include <iostream>
#include <sstream>
void Scope::AddChild(std::shared_ptr<Scope> child)
{
    children.push_back(child);
}

void Scope::AddElement(std::string id, std::shared_ptr<SymbolTableElement> element,yy::location loc)
{
    if (SymbolAlreadyDeclared(id))
    {
        //FIXME: Stringstream on error mesages looks unnecessary
        std::stringstream s;
        s << "Multiple definition of symbol '" << id << "'!";
        Error::ShowError(s.str(),loc);
    }
    else
    {
        elements[id] = element;
    }
}

bool Scope::SymbolAlreadyDeclared(std::string id)
{
    return elements.find(id) != elements.end();
}