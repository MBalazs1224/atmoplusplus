#include "ir_call.hh"

IRCall::IRCall(std::shared_ptr<IRExpression> f, std::shared_ptr<IRExpressionList> a) : func(std::move(f)), args(std::move(a))
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