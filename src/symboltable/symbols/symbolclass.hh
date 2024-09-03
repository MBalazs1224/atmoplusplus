#pragma once
#include "../symboltableelement.hh"
#include "symbolvariable.hh"
#include "symbolfunction.hh"
#include "../../ast/nodes/body_node.hh"
#include <memory>
#include <vector>
#include "../../ast/types/type.hh"
class ClassSymbol : public SymbolTableElement, public Type, /*std::enable_shared_from_this makes it possible to retrieve a shared_ptr from this that the GetType can return*/ public std::enable_shared_from_this<ClassSymbol>
{
    private:
        std::vector<std::shared_ptr<ClassSymbol>> parents;
        std::unique_ptr<BodyNode> body;
    public:
    ClassSymbol(std::vector<std::shared_ptr<ClassSymbol>> parents_in, std::unique_ptr<BodyNode> body_in) : parents(std::move(parents_in)), body(std::move(body_in))
    {
        
    }

    std::shared_ptr<Type> GetType() override
    {
        return shared_from_this();
    }
};