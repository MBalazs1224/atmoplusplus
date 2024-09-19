#pragma once
#include "expressionable.hh"
#include "../types/type.hh"
#include "../../symboltable/symboltableelement.hh"
#include <memory>
#include <string>
#include <assert.h>
// Type because it can be a class
class Identifier : public IExpressionable, public Type
{
    private:
        // The element with this id if found
        std::shared_ptr<SymbolTableElement> element;

        

    public:
    // The name of the wanted symbol
    std::string name;
    Identifier(std::shared_ptr<SymbolTableElement> element_in, std::string name_in, yy::location loc) : IExpressionable(loc), element(std::move(element_in)),name(name_in)
    {

    }

    void SetElement(std::shared_ptr<SymbolTableElement> element)
    {
        this->element =std::move(element);
    }

    std::shared_ptr<SymbolTableElement> GetElement()
    {
        return element;
    }

    bool Equals(const std::shared_ptr<Type> other) override
    {
        return other->Equals(element->GetType());
    }

    std::string ToString() override
    {
        return element->GetType()->ToString();
    }

    bool Check() override
    {
        if(!element)
        {
            Error::ShowError(Error::FormatString("Unknown identifier '%s'!",name.c_str()),location);
        }
        return element != nullptr;
    }

    std::shared_ptr<Type> GetType() override
    {
        assert(element);
        return element->GetType();
    }

};