#include "ir_call.hh"

IRCall::IRCall(std::unique_ptr<IRExpression> f, std::unique_ptr<ExpressionList> a) : func(std::move(f)), args(std::move(a))
{

}