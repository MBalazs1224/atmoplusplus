#include "scope.hh"
#include <iostream>
void Scope::AddChild(std::shared_ptr<Scope> child)
{
    children.push_back(child);
}

void Scope::AddElement(std::string id, std::shared_ptr<SymbolTableElement> element)
{
    if (SymbolAlreadyDeclared(id))
    {
        //FIXME: Correct error message if symbol is already declared
        std::cout << "Symbol already declared!" << std::endl;
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