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
    
    current->AddElement(id,element);
}

std::shared_ptr<SymbolTableElement> SymbolTable::LookUp(const std::string& id, const yy::location& loc)
{
    assert(!id.empty());

    auto element = current->GetElement(id);

    // If there is no element found show unknown error
    if(!element)
    {
        Error::ShowError(Error::FormatString("Unknown identifier '%s'!", id.c_str()),loc);
    }

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
