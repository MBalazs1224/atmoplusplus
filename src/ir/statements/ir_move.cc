#include "ir_move.hh"

IRMove::IRMove(std::shared_ptr<IRExpression> src_in, std::shared_ptr<IRExpression> dest_in) : source(std::move(src_in)), destination(std::move(dest_in))
{

}