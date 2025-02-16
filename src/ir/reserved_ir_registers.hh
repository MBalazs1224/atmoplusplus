#pragma once
#include <memory>
#include "../frame/temp.hh"
// Will contain the reserved Temp object for rsp and rbp
class ReservedIrRegisters
{
    public:
        static std::shared_ptr<Temp> StackPointer;
        static std::shared_ptr<Temp> FramePointer;
        static std::shared_ptr<Temp> RAX;
        static std::shared_ptr<Temp> RDI;
        static void Initialize();
};