#include "ir_pop.hh"

IRPop::IRPop(std::shared_ptr<IRExpression> expression_in) : exp(expression_in)
{

}

std::string IRPop::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;
    std::string dot = "node" + std::to_string(myId) + " [label=\"POP\"];\n";

    // Connect pop target
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
    dot += exp->ToDotFormat(nodeCounter);

    return dot;
}

std::shared_ptr<IRStatement> IRPop::Build(std::shared_ptr<IRExpressionList> kids)
{
    return shared_from_this();
        
}

std::shared_ptr<IRExpressionList> IRPop::Kids()
{
    return std::make_shared<IRExpressionList>(
        this->exp,
        nullptr
    );
}