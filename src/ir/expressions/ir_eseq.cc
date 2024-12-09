#include "ir_eseq.hh"

IREseq::IREseq(std::shared_ptr<IRExpression> e, std::shared_ptr<IRStatement> s) : exp(std::move(e)), stmt(std::move(s))
{

}