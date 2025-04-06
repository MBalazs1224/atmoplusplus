#pragma once
#include <memory>
#include "../../frame/temp.hh"

class LiveInterval
{
    public:
        std::shared_ptr<Temp> temp;
        int start;
        int end;
    
        LiveInterval(std::shared_ptr<Temp> t, int s, int e) : temp(t), start(s), end(e) {}
};