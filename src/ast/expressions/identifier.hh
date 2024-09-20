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
    Identifier(std::shared_ptr<SymbolTableElement> element_in, std::string name_in, yy::location loc);

    void SetElement(std::shared_ptr<SymbolTableElement> element);

    std::shared_ptr<SymbolTableElement> GetElement();

    bool Equals(const std::shared_ptr<Type> other) override;

    std::string ToString() override;

    bool Check() override;

    std::shared_ptr<Type> GetType() override;

};