#pragma once
#include "../ast/types/type.hh"
#include "../ast/attributes/attribute.hh"
#include "../ast/attributes/attributeprivate.hh"
#include "../ast/expressions/expressionable.hh"
#include "../frame/datasize.hh"
#include "../ir/reserved_ir_registers.hh"
#include <memory>
#include <algorithm>
class SymbolTableElement : public IExpressionable
{
    protected:
    // These need to be pointers so typeid works correctly
        std::shared_ptr<Type> type;
        std::vector<std::shared_ptr<Attribute>> attributes;
    public:
        std::string name;
        SymbolTableElement()
        {
        }
        SymbolTableElement(std::shared_ptr<Type>,std::vector<std::shared_ptr<Attribute>>);

        // Constructor for a constructor/destructor function (that does not have a return type)
        SymbolTableElement(std::vector<std::shared_ptr<Attribute>>);
        virtual ~SymbolTableElement();
        std::vector<std::shared_ptr<Attribute>> GetAttributes();
        // Returns true if the element is static or public. (meaning its reachable outside the class)
        virtual bool IsReachableOutside();
        
        /*
        * @return The size of the element in bytes.
        */
        virtual DataSize GetSize() = 0;
};