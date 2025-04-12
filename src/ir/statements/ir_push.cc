#include "ir_push.hh"

IRPush::IRPush(std::shared_ptr<IRExpression> expression_in) : exp(expression_in)
{

}

std::string IRPush::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;
    std::string dot = "node" + std::to_string(myId) + " [label=\"PUSH\"];\n";

    // Connect push target
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
    dot += exp->ToDotFormat(nodeCounter);

    return dot;
}

std::shared_ptr<IRStatement> IRPush::Build(std::shared_ptr<IRExpressionList> kids)
{
    return shared_from_this();
        
}

std::shared_ptr<IRExpressionList> IRPush::Kids()
{
    return std::make_shared<IRExpressionList>(
        this->exp,
        nullptr
    );
}