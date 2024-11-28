#include "ir_sequence.hh"

IRSequence::IRSequence(std::unique_ptr<IRExpression> l, std::unique_ptr<IRExpression> r) : left(std::move(l)),right(std::move(r))
{

}