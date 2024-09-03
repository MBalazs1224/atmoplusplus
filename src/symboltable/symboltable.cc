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

std::shared_ptr<SymbolTableElement> SymbolTable::LookUp(std::string id, const yy::location& loc)
{
    assert(!id.empty());

    auto element = current->GetElement(id);

    // If there is no element found show unknown error
    if(!element)
    {
        //FIXME: Fix stringstream
        std::stringstream ss;
        ss << "Unknown identifier '" << id << "'!";
        Error::ShowError(ss.str(),loc);
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

    current->AddChild(new_scope);
    current = new_scope;
    
}

void SymbolTable::DecreaseScope()
{
    if (current->parent) 
    {
        current = current->parent;
    }
}
void SymbolTable::Dump()
{
    DumpScope(current, 0);
}

void SymbolTable::DumpScope(std::shared_ptr<Scope> scope, int level)
{
    if (!scope) return;

    std::string indent(level * 2, ' ');
    for (const auto& elem : scope->elements)
    {
        std::cout << indent << elem.first << std::endl;
    }

    for (const auto& child : scope->children)
    {
        DumpScope(child, level + 1);
    }
}

bool SymbolTable::IsRoot()
{
    return root == current;
}
