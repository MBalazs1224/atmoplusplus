#pragma once
#include "../symboltableelement.hh"
#include "../../ast/expressions/identifier.hh"
class VariableSymbol : public SymbolTableElement
{
    public:
    VariableSymbol(std::shared_ptr<Type> type_in, std::shared_ptr<Attribute> attr_in) : SymbolTableElement(type_in,std::move(attr_in))
    {

    }
    std::shared_ptr<Type> GetType() override
    {
        if (auto id = std::dynamic_pointer_cast<Identifier>(type))
        {
            return id->GetType();
        }
        
        return this->type;
    }
    bool Check() override
    {
        // FIXME: Symbols should'nt be node, i leave it here for now, later i need to redo the class hierarchy
        //TODO: Implement symbol variable checking
        return true;
    }
};
