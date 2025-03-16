#pragma once

#include <memory>
#include "frame.hh"

class GlobalFrame
{
    public:
        static std::unique_ptr<Frame> globalFrameType;

};