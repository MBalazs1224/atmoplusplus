#include "load_effective_address.hh"

IRLoadEffectiveAddress::IRLoadEffectiveAddress(std::shared_ptr<IRExpression> src_in) : source(std::move(src_in))
{

}

DataSize IRLoadEffectiveAddress::Size()
{
    return DataSize::QWord; // 64 bit address
}

std::string IRLoadEffectiveAddress::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;

    std::string dot = "node" + std::to_string(myId) + " [label=\"LOAD_EFFECTIVE_ADDRESS\"];\n";

    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";

    dot += source->ToDotFormat(nodeCounter);

    return dot;
}

std::shared_ptr<IRExpression> IRLoadEffectiveAddress::Build(std::shared_ptr<IRExpressionList> kids)
{

    return std::make_shared<IRLoadEffectiveAddress>(
        kids->expression
    );
}

std::shared_ptr<IRExpressionList> IRLoadEffectiveAddress::Kids()
{

    return std::make_shared<IRExpressionList>(
        this->source,
        nullptr
    );
}