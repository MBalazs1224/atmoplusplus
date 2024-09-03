#pragma once
#include "symboltableelement.hh"
#include <unordered_map>
#include <string>
#include <memory>
#include "scope.hh"
#include "../../location.hh"
// Class for managing symbols
class SymbolTable
{
    private:
        static std::shared_ptr<Scope> root;
        static std::shared_ptr<Scope> current;

// Make the constructor private so no one can instantiate
        SymbolTable(){};

    public:
        // Will insert the SymbolTableElement into the hashmap and add te current scope.
        static void Insert(std::string,std::shared_ptr<SymbolTableElement>);
        // Will return a reference to the SymbolTableElement or null if the element wasn't found.
        //BUG: Symbol in a scope under the current one gets recognized correctly
        static std::shared_ptr<SymbolTableElement> LookUp(const std::string&, const yy::location&);
        static void DecreaseScope();
        static void IncreaseScope();
        static bool IsRoot();
        // Debug function to dump all the contents inside the hashmap on the screen.
        static void Dump();
        static void DumpScope(std::shared_ptr<Scope> scope, int level);

        static void Initialize();
};
