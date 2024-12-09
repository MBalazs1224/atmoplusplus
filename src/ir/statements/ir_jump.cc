#include "ir_jump.hh"

IRJump::IRJump(std::shared_ptr<IRExpression> e, std::shared_ptr<LabelList> l) : exp(std::move(e)), targets(std::move(l))
{
}