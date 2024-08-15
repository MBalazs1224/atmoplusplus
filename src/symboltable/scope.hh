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
    std::vector<std::shared_ptr<Scope>> children;
    std::unordered_map<std::string, std::shared_ptr<SymbolTableElement>> elements;

    public:
    Scope(std::shared_ptr<Scope> parent_in = nullptr) : parent(parent_in)
    {

    }
    void AddChild(std::shared_ptr<Scope>);
    void AddElement(std::string, std::shared_ptr<SymbolTableElement>);
    bool SymbolAlreadyDeclared(std::string);
    std::shared_ptr<SymbolTableElement> GetElement(std::string);
    friend class SymbolTable;
};