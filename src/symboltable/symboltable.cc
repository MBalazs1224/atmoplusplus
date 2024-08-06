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
    assert(element != nullptr);
    
    //TODO: Implement SymbolTable Insert
}

std::shared_ptr<SymbolTableElement>& SymbolTable::LookUp(std::string id)
{
    assert(!id.empty());
    //TODO: Implement SymbolTable Lookup
    
}

void SymbolTable::Initialize()
{
    root = std::make_shared<Scope>();
    current = root;
}

void SymbolTable::IncreaseScope()
{
    auto new_scope = std::make_shared<Scope>();

    current->AddChild(new_scope);
    new_scope->parent = current;
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
    //TODO: Implement SymbolTable Dump
    
}

void SymbolTable::Insert(std::string id)
{
    assert(!id.empty());
    //TODO: Implement SymbolTable Insert
    //map.insert(std::make_pair(id,std::shared_ptr<SymbolTableElement>(nullptr)));
}