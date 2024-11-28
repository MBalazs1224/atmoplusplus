#include "ir_jump.hh"

IRJump::IRJump(std::unique_ptr<IRExpression> e, std::unique_ptr<LabelList> l) : exp(std::move(e)), targets(std::move(l))
{
}