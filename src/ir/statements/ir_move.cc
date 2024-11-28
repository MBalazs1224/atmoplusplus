#include "ir_move.hh"

IRMove::IRMove(std::unique_ptr<IRExpression> src_in, std::unique_ptr<IRExpression> dest_in) : source(std::move(src_in)), destination(std::move(dest_in))
{

}