#include "ilocation.hh"

ILocation::~ILocation() = default;

void ILocation::SetLocation(yy::location loc)
{
    this->location = loc;
}