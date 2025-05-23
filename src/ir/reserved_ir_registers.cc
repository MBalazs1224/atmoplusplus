#include "reserved_ir_registers.hh"

std::shared_ptr<Temp> ReservedIrRegisters::StackPointer;
std::shared_ptr<Temp> ReservedIrRegisters::FramePointer;
std::shared_ptr<Temp> ReservedIrRegisters::RAX;
std::shared_ptr<Temp> ReservedIrRegisters::RBX;
std::shared_ptr<Temp> ReservedIrRegisters::RDI;
std::shared_ptr<Temp> ReservedIrRegisters::RSI;
std::shared_ptr<Temp> ReservedIrRegisters::RDX;
std::shared_ptr<Temp> ReservedIrRegisters::RCX;
std::shared_ptr<Temp> ReservedIrRegisters::R8;
std::shared_ptr<Temp> ReservedIrRegisters::R9;
std::shared_ptr<Temp> ReservedIrRegisters::R10;
std::shared_ptr<Temp> ReservedIrRegisters::R11;
std::shared_ptr<Temp> ReservedIrRegisters::R12;
std::shared_ptr<Temp> ReservedIrRegisters::R13;
std::shared_ptr<Temp> ReservedIrRegisters::R14;
std::shared_ptr<Temp> ReservedIrRegisters::R15;

std::shared_ptr<Temp> ReservedIrRegisters::XMM0;
std::shared_ptr<Temp> ReservedIrRegisters::XMM1;
std::shared_ptr<Temp> ReservedIrRegisters::XMM2;
std::shared_ptr<Temp> ReservedIrRegisters::XMM3;
std::shared_ptr<Temp> ReservedIrRegisters::XMM4;
std::shared_ptr<Temp> ReservedIrRegisters::XMM5;
std::shared_ptr<Temp> ReservedIrRegisters::XMM6;
std::shared_ptr<Temp> ReservedIrRegisters::XMM7;

std::unordered_map<std::shared_ptr<Temp>, std::string, TempPtrHash, TempPtrEqual> ReservedIrRegisters::ReservedTempToReg;


std::vector<std::shared_ptr<Temp>> ReservedIrRegisters::incomingIntegerParameterLocations;
std::vector<std::shared_ptr<Temp>> ReservedIrRegisters::incomingFloatParameterLocations;
std::vector<std::shared_ptr<Temp>> ReservedIrRegisters::calleeSavedRegs;
std::vector<std::shared_ptr<Temp>> ReservedIrRegisters::callerSavedRegs;

void ReservedIrRegisters::Initialize()
{
    StackPointer = std::make_shared<Temp>();
    FramePointer = std::make_shared<Temp>();

    RAX = std::make_shared<Temp>();
    RBX = std::make_shared<Temp>();
    RDI  = std::make_shared<Temp>();
    RSI = std::make_shared<Temp>();
    RDX  = std::make_shared<Temp>();
    RCX  = std::make_shared<Temp>();
    R8 = std::make_shared<Temp>();
    R9  = std::make_shared<Temp>();
    R10  = std::make_shared<Temp>();
    R11  = std::make_shared<Temp>();
    R12  = std::make_shared<Temp>();
    R13  = std::make_shared<Temp>();
    R14 = std::make_shared<Temp>();
    R15 = std::make_shared<Temp>();

    XMM0  = std::make_shared<Temp>();
    XMM1 = std::make_shared<Temp>();
    XMM2  = std::make_shared<Temp>();
    XMM3 = std::make_shared<Temp>();
    XMM4  = std::make_shared<Temp>();
    XMM5 = std::make_shared<Temp>();
    XMM6  = std::make_shared<Temp>();
    XMM7 = std::make_shared<Temp>();

    // FIXME: They need to be set that the are only 4 bytes!!!!!
    incomingIntegerParameterLocations = {RDI, RSI, RDX, RCX, R8, R9};
    incomingFloatParameterLocations = {XMM0, XMM1,XMM2,XMM3,XMM4,XMM5,XMM6,XMM7};

    calleeSavedRegs = {RBX,FramePointer,R12,R13,R14,R15};
    callerSavedRegs = {RAX,RCX,RDX,RSI,RDI,R8,R9,R10,R11};


    ReservedTempToReg[StackPointer] = "rsp";
    ReservedTempToReg[FramePointer] = "rbp";

    ReservedTempToReg[RAX] = "rax";
    ReservedTempToReg[RBX] = "rbx";
    ReservedTempToReg[RDI] = "rdi";
    ReservedTempToReg[RSI] = "rsi";
    ReservedTempToReg[RDX] = "rdx";
    ReservedTempToReg[RCX] = "rcx";
    ReservedTempToReg[R8] = "r8";
    ReservedTempToReg[R9] = "r9";
    ReservedTempToReg[R10] = "r10";
    ReservedTempToReg[R11] = "r11";
    ReservedTempToReg[R12] = "r12";
    ReservedTempToReg[R13] = "r13";
    ReservedTempToReg[R14] = "r14";
    ReservedTempToReg[R15] = "r15";

    ReservedTempToReg[XMM0] = "xmm0";
    ReservedTempToReg[XMM1] = "xmm1";
    ReservedTempToReg[XMM2] = "xmm2";
    ReservedTempToReg[XMM3] = "xmm3";
    ReservedTempToReg[XMM4] = "xmm4";
    ReservedTempToReg[XMM5] = "xmm5";
    ReservedTempToReg[XMM6] = "xmm6";
    ReservedTempToReg[XMM7] = "xmm7";


}