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
        // Will insert the SymbolTableElement into the hashmap.
        static void Insert(std::string,std::shared_ptr<SymbolTableElement>);
        // Will return a reference to the SymbolTableElement or null if the element wasn't found.
        static std::shared_ptr<SymbolTableElement> LookUp(std::string);
        static void HideScope();
        static void IncreaseScope();
        // Debug function to dump all the contents inside the hashmap on the screen.
        static void Dump();
};