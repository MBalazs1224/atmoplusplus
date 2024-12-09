#include "ir_mem.hh"

IRMem::IRMem(std::shared_ptr<IRExpression> e) : exp(std::move(e))
{

}