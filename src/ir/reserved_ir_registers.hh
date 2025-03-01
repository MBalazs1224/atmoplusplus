#pragma once
#include <memory>
#include <vector>
#include "../frame/temp.hh"
// Will contain the reserved Temp object for rsp and rbp
class ReservedIrRegisters
{
    public:

    // Integer regs

        static std::shared_ptr<Temp> StackPointer;
        static std::shared_ptr<Temp> FramePointer;
        static std::shared_ptr<Temp> RAX;
        static std::shared_ptr<Temp> RBX;
        static std::shared_ptr<Temp> RDI;
        static std::shared_ptr<Temp> RSI;
        static std::shared_ptr<Temp> RDX;
        static std::shared_ptr<Temp> RCX;
        static std::shared_ptr<Temp> R8;
        static std::shared_ptr<Temp> R9;
        static std::shared_ptr<Temp> R10;
        static std::shared_ptr<Temp> R11;
        static std::shared_ptr<Temp> R12;
        static std::shared_ptr<Temp> R13;
        static std::shared_ptr<Temp> R14;
        static std::shared_ptr<Temp> R15;

    // Float regs

        static std::shared_ptr<Temp> XMM0;
        static std::shared_ptr<Temp> XMM1;
        static std::shared_ptr<Temp> XMM2;
        static std::shared_ptr<Temp> XMM3;
        static std::shared_ptr<Temp> XMM4;
        static std::shared_ptr<Temp> XMM5;
        static std::shared_ptr<Temp> XMM6;
        static std::shared_ptr<Temp> XMM7;

        // Will contain the regsiters used for incoming integer parameters in the correct order.
        static std::vector<std::shared_ptr<Temp>> incomingIntegerParameterLocations;

        // Will contain the regsiters used for incoming float parameters in the correct order.
        static std::vector<std::shared_ptr<Temp>> incomingFloatParameterLocations;

        // Will contain the callee-saved registers
        static std::vector<std::shared_ptr<Temp>> calleeSavedRegs;

        // Will contain the caller-saved registers
        static std::vector<std::shared_ptr<Temp>> callerSavedRegs;

        static void Initialize();
};