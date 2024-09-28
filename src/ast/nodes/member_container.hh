#pragma once
#include <memory>
#include "../../symboltable/symboltableelement.hh"

// Class for expressions that can refer to a symbol table element (member access, identifier etc.)
class MemberContainer
{
    public:
        // Returns the element that the expression refers to
        virtual std::shared_ptr<SymbolTableElement> GetElement() = 0;
};