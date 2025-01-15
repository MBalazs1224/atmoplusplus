#include "ir_cjump.hh"

IRCJump::IRCJump(RelationalOperator op, std::shared_ptr<IRExpression> l, std::shared_ptr<IRExpression>r, IRLabel t, IRLabel f) : relop(op), left(std::move(l)), right(std::move(r)), iftrue(t), iffalse(f)
{

}