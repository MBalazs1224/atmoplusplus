#pragma once
#include "../../frame/boollist.hh"
#include "../symboltableelement.hh"
#include "symbolvariable.hh"
#include "../../ast/nodes/body_node.hh"
#include "../../ast/expressions/identifier.hh"
#include "../../ast/attributes/attributepublic.hh"
#include <memory>
#include <vector>
#include "class_member.hh"
#include "../../frame/printedlabel.hh"

#include "../../frame/global_frame.hh"

class ClassSymbol;

class BodyNode;

class FunctionSymbol : public SymbolTableElement, public ClassMember
{
    private:
        std::vector<std::shared_ptr<VariableSymbol>> arguments;

        std::shared_ptr<BoolList>  GetWetherVariablesEscape();

        bool CheckAttributes();


    public:
        int size_in_bytes = 0;

        bool isVirtual = false;
        bool isOverriding = false;

        // Some functions have different name in assembly (like functions inside classes)
        std::string nameInAssembly;

        // Will point to the class, where this function can be found inside
        std::shared_ptr<ClassSymbol> containingClass;

        /// @brief Will return wether the function was defined inside a class.
        bool IsInClass();

        std::shared_ptr<IRExpressionList> ConvertParameterLocationToList();



    // Contains all the local variables defined in the function
    std::vector<std::shared_ptr<VariableSymbol>> local_variables;

    std::unique_ptr<BodyNode> body;
    
    FunctionSymbol(std::shared_ptr<Type> type_in, std::vector<std::shared_ptr<Attribute>> attr_in, std::vector<std::shared_ptr<VariableSymbol>> args_in, std::unique_ptr<BodyNode> body_in, std::string name_in);

    // Constructor for a constructor function
    FunctionSymbol(std::vector<std::shared_ptr<Attribute>>, std::unique_ptr<BodyNode> body_in, std::vector<std::shared_ptr<VariableSymbol>> args_in);

    // Will create an empty public function with no body or parameters 
    FunctionSymbol();

    // Will create an empty function with no parameters or attributes, mainly used to declare destructors
    FunctionSymbol(std::unique_ptr<BodyNode>);

    std::shared_ptr<Type> GetType() override;
    bool Check() override;

    // Const and reference so it can be retrevied fast but cannot be changed by other classes
    const std::vector<std::shared_ptr<VariableSymbol>>& GetArguments();

    DataSize GetSize() override;

    std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;
    std::shared_ptr<IRStatement> TranslateToIR() override;
};
