// https://steemit.com/programming/@drifter1/writing-a-simple-compiler-on-my-own-using-symbol-tables-in-the-lexer

#include "symboltable.hh"
#include <iostream>

// Initialize the private members inside the class
std::unordered_map<std::string, std::shared_ptr<SymbolTableElement>> SymbolTable::map;
int SymbolTable::current_scope = 0;

void SymbolTable::Insert(std::string id, std::shared_ptr<SymbolTableElement> element)
{
    map.insert(std::make_pair(id,element));
}

std::shared_ptr<SymbolTableElement> SymbolTable::LookUp(std::string id)
{
    auto iterator = map.find(id);
    if (iterator != map.end())
    {
        return iterator->second;
    }
    return nullptr;
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