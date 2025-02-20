#pragma once
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "../attributes/attribute.hh"
#include "../types/type.hh"
#include "../../symboltable/symbols/symbolvariable.hh"
#include "../../symboltable/symbols/symbolclass.hh"
#include "../../symboltable/symbols/symbolfunction.hh"
#include "../../ir/expressions/ir_expression_list.hh"
#include <memory>
#include <vector>

class FunctionSymbol;

class ConstructorDefinitionNode;

class VariableDefinitionNode : public Node
{
    private:
        // Pointer to the created variable
        std::shared_ptr<VariableSymbol> variable;
        

        // If the variable is a class, this will hold the arguments to the constructor
        std::vector<std::shared_ptr<IExpressionable>> arguments_to_constructor;

        // If the variable is a class and the arguments match a constructor, this will hold the constructor

        std::shared_ptr<ConstructorDefinitionNode> constructor;

        // Will check if the variable is a class
        bool VariableIsClass();

        // Will check if the constructor exists and is valid
        bool CheckConstructor();

    public:
        std::shared_ptr<IExpressionable> expression;

        VariableDefinitionNode( std::shared_ptr<VariableSymbol> var_in, std::shared_ptr<IExpressionable> exp_in, yy::location loc, std::vector<std::shared_ptr<IExpressionable>>  args);

        ~VariableDefinitionNode() override = default;

        bool Check() override;

        std::shared_ptr<VariableSymbol> GetVariable();

        std::shared_ptr<IRStatement> TranslateToIR() override;

        std::shared_ptr<IRExpressionList> TranslateArgumentsToConstructorToIR(); 



};