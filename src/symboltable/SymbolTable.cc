// https://steemit.com/programming/@drifter1/writing-a-simple-compiler-on-my-own-using-symbol-tables-in-the-lexer

#include "symboltable.hh"
#include <iostream>
#include <cassert>

// Initialize the private members inside the class
std::unordered_map<std::string, std::shared_ptr<SymbolTableElement>> SymbolTable::map;
int SymbolTable::current_scope = 0;

void SymbolTable::Insert(std::string id, std::shared_ptr<SymbolTableElement> element)
{
    assert(!id.empty());
    assert(element != nullptr);
    
    element->AddScope(current_scope);
    map.insert(std::make_pair(id,element));
}

std::shared_ptr<SymbolTableElement>& SymbolTable::LookUp(std::string id)
{
    assert(!id.empty());
    
    return map[id];
    
}
void SymbolTable::IncreaseScope()
{
    current_scope++;
}

void SymbolTable::HideScope()
{
    if(current_scope > 0)
        current_scope--;
}
void SymbolTable::Dump()
{
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        std::cout << it->first << std::endl;;
        std::cout << "------------------" << std::endl;
    }
    
}

void SymbolTable::Insert(std::string id)
{
    assert(!id.empty());

    map.insert(std::make_pair(id,std::shared_ptr<SymbolTableElement>(nullptr)));
}

bool SymbolTable::SymbolIsValid(std::string id)
{
    assert(!id.empty());
    auto iterator = map.find(id);
    return iterator != map.end();
}

bool SymbolTable::SymbolAlreadyDeclared(std::string id)
{
    return map[id] != nullptr;
}

void SymbolTable::Switch(std::string id, std::shared_ptr<SymbolTableElement> element)
{
    map[id] = element;
}