#pragma once
#include "../symboltableelement.hh"
#include "symbolvariable.hh"
#include "../../ast/nodes/body_node.hh"
#include "../../ast/expressions/identifier.hh"
#include "../../ast/attributes/attributepublic.hh"
#include <memory>
#include <vector>
class FunctionSymbol : public SymbolTableElement
{
    private:
        std::vector<std::shared_ptr<VariableSymbol>> arguments;
    public: 

    std::unique_ptr<BodyNode> body;
    
    FunctionSymbol(std::shared_ptr<Type> type_in, std::shared_ptr<Attribute> attr_in, std::vector<std::shared_ptr<VariableSymbol>> args_in, std::unique_ptr<BodyNode> body_in);

    // Constructor for a constructor function
    FunctionSymbol(std::shared_ptr<Attribute> attr_in, std::unique_ptr<BodyNode> body_in, std::vector<std::shared_ptr<VariableSymbol>> args_in);

    // Will create an empty public function with no body or parameters 
    FunctionSymbol();

    std::shared_ptr<Type> GetType() override;
    bool Check() override;

    // Const and reference so it can be retrevied fast but cannot be changed by other classes
    const std::vector<std::shared_ptr<VariableSymbol>>& GetArguments();
};
