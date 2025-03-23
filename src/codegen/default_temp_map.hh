#pragma once
#include "tempmap.hh"
// Default temp map, returns the temp's id as it's name, used for debugging
class DefaultTempMap : public TempMap
{
    std::string Map(std::shared_ptr<Temp> t) override
    {
        return t->ToString();
    }
};