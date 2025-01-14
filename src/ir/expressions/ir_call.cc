#include "ir_call.hh"

IRCall::IRCall(std::shared_ptr<IRExpression> f, std::shared_ptr<IRExpressionList> a) : func(std::move(f)), args(std::move(a))
{

}