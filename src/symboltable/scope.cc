#include "scope.hh"

void Scope::AddChild(std::shared_ptr<Scope> child)
{
    children.push_back(child);
}