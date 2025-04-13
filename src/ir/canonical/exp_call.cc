#include "exp_call.hh"


std::shared_ptr<IRExpressionList> IRExpCall::Kids()
{
    return call->Kids();
}
std::shared_ptr<IRStatement> IRExpCall::Build(std::shared_ptr<IRExpressionList> kids)
{
    return std::make_shared<IREvaluateExpression>(
        this->call->Build(kids)
    );
}

IRExpCall::IRExpCall(std::shared_ptr<IRCall> callIn)
{
    this->call = callIn;
}

std::string IRExpCall::ToDotFormat(int& counter)
{
    throw "ToDotFormat inside IRExpCall was called!";
}