#pragma once
#include "node.hh"
#include "body_node.hh"
#include "../../symboltable/symbols/symbolfunction.hh"
class ConstructorDefinitionNode : public Node
{
    private:

    
    // The arguments to parent for correct chaining
    std::vector<std::shared_ptr<IExpressionable>> arguments_to_parent;
    
    public:

    // The constructor which the arguments need to be given to

    std::shared_ptr<ConstructorDefinitionNode> constructorOfParent;

    // The function symbol of the constructor
    std::shared_ptr<FunctionSymbol> function;

    ConstructorDefinitionNode(std::shared_ptr<FunctionSymbol> function_in, std::vector<std::shared_ptr<IExpressionable>>, yy::location);

    // For default empty constructors in classes, will create an empty FunctionSymbol for the function
    ConstructorDefinitionNode(yy::location location);


    ~ConstructorDefinitionNode() override;
    bool Check() override;
    const std::vector<std::shared_ptr<IExpressionable>>& GetArgumentsForParentConstructor();
    std::shared_ptr<FunctionSymbol> GetFunction();

    std::shared_ptr<IRStatement> TranslateToIR() override;


    /*
    @brief Will translate the constructor into IR statements and call the parent constructor if needed.
    @param locationOfClassObject The location of the class object, so it will know where the variables are.
    @param paramsPassedToConstructor The parameters passed to this constructor.
    @return The chained parent costructor calls.
    */
    std::shared_ptr<IRStatement> TranslateToIRWithGivenParemeter(std::shared_ptr<IRExpression> locationOfClassObject, std::shared_ptr<IRExpressionList> paramsPassedToConstructor);

    std::shared_ptr<IRExpressionList> TranslateArgumentsToParentToIR();
};