#pragma once
#include "../symboltableelement.hh"
#include "symbolvariable.hh"
#include "../../ast/nodes/body_node.hh"
#include "../../ast/expressions/identifier.hh"
#include <memory>
#include <vector>
class FunctionSymbol : public SymbolTableElement
{
    private:
        std::vector<std::shared_ptr<VariableSymbol>> arguments;
        std::unique_ptr<BodyNode> body;
    public:
    FunctionSymbol(std::shared_ptr<Type> type_in, std::shared_ptr<Attribute> attr_in, std::vector<std::shared_ptr<VariableSymbol>> args_in, std::unique_ptr<BodyNode> body_in) : SymbolTableElement(type_in,std::move(attr_in))
    {
        arguments = args_in;
        body = std::move(body_in);
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
        //TODO: Implement symbol function checking
        return true;
    }
};
