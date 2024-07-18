#pragma once
#include "SymbolTableElement.hh"
#include <unordered_map>
#include <string>
#include <memory>
// Class for managing symbols
class SymbolTable
{
    private:
        static std::unordered_map<std::string,std::shared_ptr<SymbolTableElement>> map;
        static int current_scope;

// Make the constructor private so no one can instantiate
        SymbolTable(){};

    public:
        // Will insert the SymbolTableElement into the hashmap and add te current scope.
        static void Insert(std::string,std::shared_ptr<SymbolTableElement>);
        // Will isnert a dummy element into the hasmap (for when the lexer doesn't know if the symbol is a function or a variable)
        static void Insert(std::string);
        // Will return a reference to the SymbolTableElement or null if the element wasn't found.
        static std::shared_ptr<SymbolTableElement>& LookUp(std::string);
        static void HideScope();
        static void IncreaseScope();
        // Debug function to dump all the contents inside the hashmap on the screen.
        static void Dump();
        static bool SymbolIsValid(std::string);
        static bool SymbolAlreadyDeclared(std::string);
        static void Switch(std::string, std::shared_ptr<SymbolTableElement>);
};