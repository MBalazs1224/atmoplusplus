#pragma once
#include "../symboltableelement.hh"
#include "../argument.hh"
#include "../../ast/nodes/body_node.hh"
#include <memory>
#include <vector>
class FunctionSymbol : public SymbolTableElement
{
    private:
        std::vector<std::shared_ptr<Argument>> arguments;
        std::unique_ptr<BodyNode> body;
    public:
    FunctionSymbol(std::unique_ptr<Type> type_in, std::unique_ptr<Attribute> attr_in, std::vector<std::shared_ptr<Argument>> args_in, std::unique_ptr<BodyNode> body_in) : SymbolTableElement(std::move(type_in),std::move(attr_in))
    {
        arguments = args_in;
        body = std::move(body_in);
    }
};
