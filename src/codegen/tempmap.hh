#pragma once
#include <memory>
#include <string>
#include "../frame/temp.hh"
class TempMap
{
    public:
        virtual std::string Map(std::shared_ptr<Temp>) = 0;
};