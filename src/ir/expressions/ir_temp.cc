#include "ir_temp.hh"

IRTemp::IRTemp(std::shared_ptr<Temp> t) : temp(std::move(t))
{

}