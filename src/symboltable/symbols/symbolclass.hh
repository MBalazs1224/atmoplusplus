#pragma once
#include "../symboltableelement.hh"
#include "symbolvariable.hh"
#include "symbolfunction.hh"
#include "../../ast/nodes/body_node.hh"
#include <memory>
#include <vector>
#include "../../ast/types/type.hh"
#include "../../ast/nodes/variable_definition_node.hh"
#include "../../ast/nodes/function_definition_node.hh"
#include "../../ast/nodes/constructor_definition_node.hh"
#include "../../ast/nodes/destructor_definition_node.hh"
#include "../../ast/expressions/identifier.hh"
#include "../../frame/offset_from_object.hh"
#include "../../codegen/global_strings.hh"
#include "../../driver/vtable_info.hh"
#include <unordered_map>

#include "../../driver/global_vtables.hh"



// We have to pre-define the VariableDefinitionNode because they both include each other so it ends up in a loop
class VariableDefinitionNode;

class ConstructorDefinitionNode;

class DestructorDefinitionNode;

class BodyNode;

class FunctionDefinitionNode;

class ClassSymbol : public SymbolTableElement, public Type, /*std::enable_shared_from_this makes it possible to retrieve a shared_ptr from this that the GetType can return*/ public std::enable_shared_from_this<ClassSymbol>
{
    private:
        // Counter that will keep track of the offset of the next avaialble space
        int variableOffset = 0;
        // To prevent multiple checking when accessing multiple things inside the same class type
        bool alreadyChecked = false;

        //The result of the first check so we can return it again if the class symbol is used later
        bool checkedResult;

        std::shared_ptr<VtableInfo> vtable;


        std::vector<std::shared_ptr<Identifier>> parents;

        // Points to the parent class after semantic analyzing
        std::shared_ptr<ClassSymbol> parent;

        // The constructors of the class
        std::vector<std::shared_ptr<ConstructorDefinitionNode>> constructors;

        // A class can only have one destructor so a pointer is enough
        std::shared_ptr<DestructorDefinitionNode> destructor = nullptr;

        // The hashmap storing the variables inside the class
        std::unordered_map<std::string, std::shared_ptr<VariableSymbol>> variables;

        // The hashmap storing the functions inside the class
        std::unordered_map<std::string, std::shared_ptr<FunctionSymbol>> functions;

        // Stores statements that all constructors need to execute (moving function ppointers to the correct location etc.)
        std::vector<std::shared_ptr<IRStatement>> statementsForConstructors;

        std::string GetTypeDescriptorString();


        // Will check if the parents are valid and inherit the variables and functions from the parent classes
        bool CheckParents();

        // Will check if the body is valid and process the variables and functions
        bool ProcessBody();

        // Will verify the constructors and destructor
        bool CheckConstructorsAndDestructor();

        // Will check if the identifier is already declared in the class (either as a variable or a function)
        bool IdentifierAlreadyDeclared(const std::string& id);

        std::vector<std::shared_ptr<FunctionSymbol>> GetBaseFunctionForOverride(std::shared_ptr<FunctionSymbol>);

        

        // Will return all the variables in this class
        std::unordered_map<std::string, std::shared_ptr<VariableSymbol>> GetVariables();

        // WIll return all the functions in this class
        std::unordered_map<std::string, std::shared_ptr<FunctionSymbol>> GetFunctions();

    public:

    /// @brief Will set wether the variables inside this class should use RDI as their offset.
    /// @param shouldUseRDI true: use RDI - false: Use regular offset
    void VariablesShouldUseRDI(bool shouldUseRDI);

    int size_in_bytes = 0;


    // The label that will point to the type descriptor string.
    std::shared_ptr<Label> typeDescriptorLabel;

    ClassSymbol(std::vector<std::shared_ptr<Identifier>> parents_in, std::unique_ptr<BodyNode> body_in);
    
    std::vector<std::shared_ptr<Node>> body;



    std::shared_ptr<Type> GetType() override;

    bool Compatible(const std::shared_ptr<Type> other) override;

    std::string ToString() override;

    bool Check() override;

    // Will return the function based on the given ID or null if it wasn't found
    std::shared_ptr<FunctionSymbol> GetFunction(const std::string& id);

    // Will return the variable based on the given ID or null if it wasn't found
    std::shared_ptr<VariableSymbol> GetVariable(const std::string&);

    // Will insert the variable into the class, returns false if the variable is already declared
    bool InsertVariable(const std::shared_ptr<VariableDefinitionNode>& node);

        // Will insert the function into the class, returns false if the function is already declared
     bool InsertFunction(const std::shared_ptr<FunctionDefinitionNode>& node);
    
    // Will return the element either if it's a variable or a function
    std::shared_ptr<SymbolTableElement> GetElement(const std::string&);

    // Will check if the given class is a parent of this class
    bool IsClassAParent(const std::shared_ptr<ClassSymbol>& parent);

    // Will that could be called with the given parameters
    const std::vector<std::shared_ptr<ConstructorDefinitionNode>> GetConstructorsWithParametersMatching(const std::vector<std::shared_ptr<IExpressionable>>&);

    DataSize GetSize() override;

    std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;
    std::shared_ptr<IRStatement> TranslateToIR() override;
};