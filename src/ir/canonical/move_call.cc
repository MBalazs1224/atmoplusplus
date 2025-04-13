#include "move_call.hh"


std::shared_ptr<IRExpressionList> IRMoveCall::Kids()
{
    return source->Kids();
}
std::shared_ptr<IRStatement> IRMoveCall::Build(std::shared_ptr<IRExpressionList> kids)
{
    return std::make_shared<IRMove>(
        this->destination,
        source->Build(kids)
    );
}

IRMoveCall::IRMoveCall(std::shared_ptr<IRTemp> destIn, std::shared_ptr<IRCall> sourceIn)
{
    this->destination = destIn;
    this->source = sourceIn;
}

std::string IRMoveCall::ToDotFormat(int& counter)
{
    throw "ToDotFormat inside IRMoveCall was called!";
}