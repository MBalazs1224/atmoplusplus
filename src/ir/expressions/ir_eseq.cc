#include "ir_eseq.hh"

IREseq::IREseq(std::shared_ptr<IRExpression> e, std::shared_ptr<IRStatement> s) : exp(std::move(e)), stmt(std::move(s))
{

}

std::string IREseq::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;
    std::string dot = "node" + std::to_string(myId) + " [label=\"ESEQ\"];\n";
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
    dot += stmt->ToDotFormat(nodeCounter);
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
    dot += exp->ToDotFormat(nodeCounter);
    return dot;
}

std::shared_ptr<IRExpression> IREseq::Build(std::shared_ptr<IRExpressionList> kids)
{
    throw std::logic_error("Build() not applicable to IREseq");
        
}

std::shared_ptr<IRExpressionList> IREseq::Kids()
{
    throw std::logic_error("Kids() not applicable to IREseq");
}

DataSize IREseq::Size()
{
    return this->exp->Size();
}