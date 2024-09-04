#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "symboltableelement.hh"
#include "../../location.hh"
#include "../error/error.hh"
class Scope
{
    std::shared_ptr<Scope> parent;
    std::unordered_map<std::string, std::shared_ptr<SymbolTableElement>> elements;

    public:
    Scope(std::shared_ptr<Scope> parent_in = nullptr) : parent(parent_in)
    {

    }
    void AddElement(const std::string&, std::shared_ptr<SymbolTableElement>);
    bool SymbolAlreadyDeclared(const std::string&);
    std::shared_ptr<SymbolTableElement> GetElement(const std::string&);
    friend class SymbolTable;
};