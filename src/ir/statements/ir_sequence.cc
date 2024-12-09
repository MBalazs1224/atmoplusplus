#include "ir_sequence.hh"

IRSequence::IRSequence(std::shared_ptr<IRExpression> l, std::shared_ptr<IRExpression> r) : left(std::move(l)),right(std::move(r))
{

}