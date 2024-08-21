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
        std::vector<std::shared_ptr<VariableSymbol>> variables;
        std::vector<std::shared_ptr<FunctionSymbol>> functions;
    public:
    ClassSymbol(std::vector<std::shared_ptr<ClassSymbol>> parents_in, std::vector<std::shared_ptr<VariableSymbol>> vars, std::vector<std::shared_ptr<FunctionSymbol>> funcs)
    {
        parents = std::move(parents_in);
        variables = std::move(vars);
        functions = std::move(funcs);
    }
};