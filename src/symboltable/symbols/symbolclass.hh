#pragma once
#include "../symboltableelement.hh"
#include "symbolvariable.hh"
#include "symbolfunction.hh"
#include "../../ast/nodes/body_node.hh"
#include <memory>
#include <vector>
class ClassSymbol : public SymbolTableElement
{
    private:
        std::vector<std::shared_ptr<ClassSymbol>> parents;
        std::unique_ptr<BodyNode> body;
    public:
    ClassSymbol(std::vector<std::shared_ptr<ClassSymbol>> parents_in, std::unique_ptr<BodyNode> body_in) : parents(std::move(parents_in)), body(std::move(body_in))
    {
        
    }
};