#pragma once
#include <memory>
#include "../../frame/temp.hh"

/// @brief Maps a virtual temporary to a physical register.
class AllocatedReg
{
    public:
        std::shared_ptr<Temp> temp;
        std::string physicalReg;

    AllocatedReg(std::shared_ptr<Temp> t, std::string r) : temp(t), physicalReg(r) {}
}; 
