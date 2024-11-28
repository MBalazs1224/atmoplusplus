#include "ir_eseq.hh"

IREseq::IREseq(std::unique_ptr<IRExpression> e, std::unique_ptr<IRStatement> s) : exp(std::move(e)), stmt(std::move(s))
{

}