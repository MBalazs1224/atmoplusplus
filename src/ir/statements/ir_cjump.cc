#include "ir_cjump.hh"

IRCJump::IRCJump(std::unique_ptr<IRExpression> l, std::unique_ptr<IRExpression>r, IRLabel t, IRLabel f) : left(std::move(l)), right(std::move(r)), iftrue(t), iffalse(f)
{

}