#pragma once
#include "tempmap.hh"
// Default temp map, returns the temp's id as it's name, used for debugging
class DefaultTempMap : public TempMap
{
    std::string Map(std::shared_ptr<Temp> t) override
    {
        if(t == ReservedIrRegisters::RAX)
        {
            return "rax";
        }
        else if (t == ReservedIrRegisters::RBX)
        {
            return "rbx";
        }
        else if (t == ReservedIrRegisters::RCX)
        {
            return "rcx";
        }
        else if (t == ReservedIrRegisters::RDX)
        {
            return "rdx";
        }
        else if (t == ReservedIrRegisters::RDI)
        {
            return "rdi";
        }
        else if (t == ReservedIrRegisters::RSI)
        {
            return "rsi";
        }
        else if (t == ReservedIrRegisters::StackPointer)
        {
            return "rsp";
        }
        else if (t == ReservedIrRegisters::FramePointer)
        {
            return "rbp";
        }
        else if (t == ReservedIrRegisters::R8)
        {
            return "r8";
        }
        else if (t == ReservedIrRegisters::R9)
        {
            return "r9";
        }
        else if (t == ReservedIrRegisters::R10)
        {
            return "r10";
        }
        else if (t == ReservedIrRegisters::R11)
        {
            return "r11";
        }
        else if (t == ReservedIrRegisters::R12)
        {
            return "r12";
        }
        else if (t == ReservedIrRegisters::R13)
        {
            return "r13";
        }
        else if (t == ReservedIrRegisters::R14)
        {
            return "r14";
        }
        else if (t == ReservedIrRegisters::R15)
        {
            return "r15";
        }

        return t->ToString();
    }
};