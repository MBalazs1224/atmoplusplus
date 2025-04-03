#pragma once
#include "tempmap.hh"
// Default temp map, returns the temp's id as it's name, used for debugging
class DefaultTempMap : public TempMap
{
    std::string Map(std::shared_ptr<Temp> t, int size_bytes) override
    {
        if (t == ReservedIrRegisters::RAX)
        {
            switch(size_bytes) {
                case 1: return "al";      // 8-bit (byte)
                case 2: return "ax";      // 16-bit (word)
                case 4: return "eax";      // 32-bit (double word)
                case 8: return "rax";      // 64-bit (quad word)
                default: return "rax";     // default to 64-bit
            }
        }
        else if (t == ReservedIrRegisters::RBX)
        {
            switch(size_bytes) {
                case 1: return "bl";
                case 2: return "bx";
                case 4: return "ebx";
                case 8: return "rbx";
                default: return "rbx";
            }
        }
        else if (t == ReservedIrRegisters::RCX)
        {
            switch(size_bytes) {
                case 1: return "cl";
                case 2: return "cx";
                case 4: return "ecx";
                case 8: return "rcx";
                default: return "rcx";
            }
        }
        else if (t == ReservedIrRegisters::RDX)
        {
            switch(size_bytes) {
                case 1: return "dl";
                case 2: return "dx";
                case 4: return "edx";
                case 8: return "rdx";
                default: return "rdx";
            }
        }
        else if (t == ReservedIrRegisters::RDI)
        {
            switch(size_bytes) {
                case 1: return "dil";
                case 2: return "di";
                case 4: return "edi";
                case 8: return "rdi";
                default: return "rdi";
            }
        }
        else if (t == ReservedIrRegisters::RSI)
        {
            switch(size_bytes) {
                case 1: return "sil";
                case 2: return "si";
                case 4: return "esi";
                case 8: return "rsi";
                default: return "rsi";
            }
        }
        else if (t == ReservedIrRegisters::StackPointer)
        {
            switch(size_bytes) {
                case 2: return "sp";
                case 4: return "esp";
                case 8: return "rsp";
                default: return "rsp";
            }
        }
        else if (t == ReservedIrRegisters::FramePointer)
        {
            switch(size_bytes) {
                case 2: return "bp";
                case 4: return "ebp";
                case 8: return "rbp";
                default: return "rbp";
            }
        }
        else if (t == ReservedIrRegisters::R8)
        {
            switch(size_bytes) {
                case 1: return "r8b";
                case 2: return "r8w";
                case 4: return "r8d";
                case 8: return "r8";
                default: return "r8";
            }
        }
        else if (t == ReservedIrRegisters::R9)
        {
            switch(size_bytes) {
                case 1: return "r9b";
                case 2: return "r9w";
                case 4: return "r9d";
                case 8: return "r9";
                default: return "r9";
            }
        }
        else if (t == ReservedIrRegisters::R10)
        {
            switch(size_bytes) {
                case 1: return "r10b";
                case 2: return "r10w";
                case 4: return "r10d";
                case 8: return "r10";
                default: return "r10";
            }
        }
        else if (t == ReservedIrRegisters::R11)
        {
            switch(size_bytes) {
                case 1: return "r11b";
                case 2: return "r11w";
                case 4: return "r11d";
                case 8: return "r11";
                default: return "r11";
            }
        }
        else if (t == ReservedIrRegisters::R12)
        {
            switch(size_bytes) {
                case 1: return "r12b";
                case 2: return "r12w";
                case 4: return "r12d";
                case 8: return "r12";
                default: return "r12";
            }
        }
        else if (t == ReservedIrRegisters::R13)
        {
            switch(size_bytes) {
                case 1: return "r13b";
                case 2: return "r13w";
                case 4: return "r13d";
                case 8: return "r13";
                default: return "r13";
            }
        }
        else if (t == ReservedIrRegisters::R14)
        {
            switch(size_bytes) {
                case 1: return "r14b";
                case 2: return "r14w";
                case 4: return "r14d";
                case 8: return "r14";
                default: return "r14";
            }
        }
        else if (t == ReservedIrRegisters::R15)
        {
            switch(size_bytes) {
                case 1: return "r15b";
                case 2: return "r15w";
                case 4: return "r15d";
                case 8: return "r15";
                default: return "r15";
            }
        }

        return t->ToString();
    }
};