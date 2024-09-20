// https://steemit.com/programming/@drifter1/writing-a-simple-compiler-on-my-own-using-symbol-tables-in-the-lexer

#include "symboltable.hh"
#include <iostream>
#include <cassert>

//Initialize private members
std::shared_ptr<Scope> SymbolTable::root;
std::shared_ptr<Scope> SymbolTable::current;

void SymbolTable::Insert(std::string id, std::shared_ptr<SymbolTableElement> element)
{
    assert(!id.empty());
    assert(element);
    element->name = id;

    if(IsClass(element) && !IsRoot())
    {
        Error::ShowError("Classes can only be created on the root level!",element->location);
        return;
    }

    
    current->AddElement(id,element);
}

bool SymbolTable::IsClass(std::shared_ptr<SymbolTableElement> element)
{
    return std::dynamic_pointer_cast<ClassSymbol>(element) != nullptr;
}

std::shared_ptr<SymbolTableElement> SymbolTable::LookUp(const std::string& id)
{
    assert(!id.empty());

    auto element = current->GetElement(id);

    return element;
}

void SymbolTable::Initialize()
{
    root = std::make_shared<Scope>();
    current = root;
}

void SymbolTable::IncreaseScope()
{
    auto new_scope = std::make_shared<Scope>(current);
    current = new_scope;
    
}

void SymbolTable::DecreaseScope()
{
    if (current->parent) 
    {
        current = current->parent;
    }
}

bool SymbolTable::IsRoot()
{
    return root == current;
}
