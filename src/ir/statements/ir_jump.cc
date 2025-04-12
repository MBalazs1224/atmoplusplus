#include "ir_jump.hh"

IRJump::IRJump(std::shared_ptr<IRExpression> e, std::shared_ptr<LabelList> l) : exp(std::move(e)), targets(std::move(l))
{
}


IRJump::IRJump(std::shared_ptr<IRName> name) : IRJump(name, std::make_shared<LabelList>(name->label,nullptr))
{

}


std::string IRJump::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    std::string dot = "node" + std::to_string(myId) + " [label=\"JUMP\"];\n";

    // Connect jump target
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
    dot += exp->ToDotFormat(nodeCounter);

    return dot;
}

std::shared_ptr<IRStatement> IRJump::Build(std::shared_ptr<IRExpressionList> kids)
{
    return std::make_shared<IRJump>(
        kids->expression,
        this->targets
    );
}

std::shared_ptr<IRExpressionList> IRJump::Kids()
{
    return std::make_shared<IRExpressionList>(
        exp,
        nullptr
    );
}