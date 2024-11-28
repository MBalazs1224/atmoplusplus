#include "ir_mem.hh"

IRMem::IRMem(std::unique_ptr<IRExpression> e) : exp(std::move(e))
{

}