#include "ir_evaluate_expression.hh"

IREvaluateExpression::IREvaluateExpression(std::shared_ptr<IRExpression> e) : exp(std::move(e))
{

}

std::string IREvaluateExpression::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    std::string dot = "node" + std::to_string(myId) + " [label=\"EVAL\"];\n";

    // Connect evaluated expression
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
    dot += exp->ToDotFormat(nodeCounter);

    return dot;
}

std::shared_ptr<IRStatement> IREvaluateExpression::Build(std::shared_ptr<IRExpressionList> kids)
{
    return std::make_shared<IREvaluateExpression>(
        kids->expression
    );
}

std::shared_ptr<IRExpressionList> IREvaluateExpression::Kids()
{
    return std::make_shared<IRExpressionList>(
        exp,
        nullptr
    );
}