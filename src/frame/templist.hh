#pragma once
#include <memory>
#include "temp.hh"
class TempList
{
    public:
        std::shared_ptr<Temp> head;
        std::shared_ptr<TempList> tail;
        TempList(std::shared_ptr<Temp> headIn, std::shared_ptr<TempList> tailIn) : head(headIn), tail(tailIn){}
};