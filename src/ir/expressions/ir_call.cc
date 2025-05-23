#include "ir_call.hh"

IRCall::IRCall(std::shared_ptr<IRExpression> f, std::shared_ptr<IRExpressionList> a, std::shared_ptr<IRExpressionList> argLocations, bool valueNeeded, DataSize retSize) : func(std::move(f)), args(std::move(a)), argumentLocations(argLocations), returnValueNeeded(valueNeeded), returnSize(retSize)
{

}

std::string IRCall::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;

    std::string dot = "node" + std::to_string(myId) + " [label=\"CALL\"];\n";

   // Connect function expression

    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";

    dot += func->ToDotFormat(nodeCounter);

   // Connect arguments (if any)
    if (args)
    {
        dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
        dot += args->ToDotFormat(nodeCounter);
    }

   return dot;
}

std::shared_ptr<IRExpression> IRCall::Build(std::shared_ptr<IRExpressionList> kids)
{
    return std::make_shared<IRCall>(
        kids->expression,
        kids->next,
        this->argumentLocations,
        false,
        kids->expression->Size()
    );
}

std::shared_ptr<IRExpressionList> IRCall::Kids()
{
    return std::make_shared<IRExpressionList>(
        this->func,
        this->args
    );
}

DataSize IRCall::Size()
{
    return this->returnSize;
}