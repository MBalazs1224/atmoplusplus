#include "boollist.hh"

BoolList::BoolList(bool head_in, std::shared_ptr<BoolList> tail_in) : head(head_in), tail(tail_in)
{
}

