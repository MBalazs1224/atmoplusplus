#include "ir_move.hh"

IRMove::IRMove(std::shared_ptr<IRExpression> dest_in, std::shared_ptr<IRExpression> src_in) : source(std::move(src_in)), destination(std::move(dest_in))
{

}

std::string IRMove::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;

    std::string dot = "node" + std::to_string(myId) + " [label=\"MOVE (SRC, DST)\"];\n";

    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";

    dot += source->ToDotFormat(nodeCounter);

    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";

    dot += destination->ToDotFormat(nodeCounter);

    return dot;
}

std::shared_ptr<IRStatement> IRMove::Build(std::shared_ptr<IRExpressionList> kids)
{
    if (auto destMem = std::dynamic_pointer_cast<IRMem>(this->destination))
    {
        return std::make_shared<IRMove>(
            std::make_shared<IRMem>(
                kids->expression,
                destMem->bytesNeeded
            ),
            kids->next->expression
        );
    }

    return std::make_shared<IRMove>(
        this->destination,
        kids->expression
    );
        
}

std::shared_ptr<IRExpressionList> IRMove::Kids()
{
    if (auto destMem = std::dynamic_pointer_cast<IRMem>(this->destination))
    {
        return std::make_shared<IRExpressionList>(
            destMem->exp,
            std::make_shared<IRExpressionList>(
                this->source,
                nullptr
            )
        );
    }

    return std::make_shared<IRExpressionList>(
        this->source,
        nullptr
    );
}