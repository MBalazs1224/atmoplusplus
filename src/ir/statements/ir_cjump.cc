#include "ir_cjump.hh"

IRCJump::IRCJump(std::shared_ptr<IRExpression> l, std::shared_ptr<IRExpression>r, IRLabel t, IRLabel f) : left(std::move(l)), right(std::move(r)), iftrue(t), iffalse(f)
{

}