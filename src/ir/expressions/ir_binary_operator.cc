#include "ir_binary_operator.hh"

IRBinaryOperator::IRBinaryOperator(BinaryOperator o, std::shared_ptr<IRExpression> l,std::shared_ptr<IRExpression> r) : binop(o), left(std::move(l)), right(std::move(r))
{

}

std::string IRBinaryOperator::ToDotFormat(int &nodeCounter)
    {
        int myId = nodeCounter++;
        std::string dot = "node" + std::to_string(myId) + " [label=\"BINOP: " + OperatorToString() + "\"];\n";

        dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
        dot += left->ToDotFormat(nodeCounter);

        dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
        dot += right->ToDotFormat(nodeCounter);

        return dot;
    }

std::string IRBinaryOperator::OperatorToString()
{
    switch (binop)
    {
        case BinaryOperator::PLUS:
            return "PLUS";
        case BinaryOperator::MINUS:
            return "MINUS";
        case BinaryOperator::MULTIPLY:
            return "MULTIPLY";
        case BinaryOperator::DIVIDE:
            return "DIVIDE";
        case BinaryOperator::OR:
            return "OR";
        case BinaryOperator::AND:
            return "AND";

        default:
            throw std::logic_error("Unknown BinaryOperator value!");
    }
}