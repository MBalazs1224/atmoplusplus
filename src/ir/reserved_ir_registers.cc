#include "reserved_ir_registers.hh"

std::shared_ptr<Temp> ReservedIrRegisters::StackPointer;
std::shared_ptr<Temp> ReservedIrRegisters::FramePointer;
std::shared_ptr<Temp> ReservedIrRegisters::RAX;
std::shared_ptr<Temp> ReservedIrRegisters::RDI;
std::shared_ptr<Temp> ReservedIrRegisters::RSI;
std::shared_ptr<Temp> ReservedIrRegisters::RDX;
std::shared_ptr<Temp> ReservedIrRegisters::RCX;
std::shared_ptr<Temp> ReservedIrRegisters::R8;
std::shared_ptr<Temp> ReservedIrRegisters::R9;

std::shared_ptr<Temp> ReservedIrRegisters::XMM0;
std::shared_ptr<Temp> ReservedIrRegisters::XMM1;
std::shared_ptr<Temp> ReservedIrRegisters::XMM2;
std::shared_ptr<Temp> ReservedIrRegisters::XMM3;
std::shared_ptr<Temp> ReservedIrRegisters::XMM4;
std::shared_ptr<Temp> ReservedIrRegisters::XMM5;
std::shared_ptr<Temp> ReservedIrRegisters::XMM6;
std::shared_ptr<Temp> ReservedIrRegisters::XMM7;


std::vector<std::shared_ptr<Temp>> ReservedIrRegisters::incomingIntegerParameterLocations;
std::vector<std::shared_ptr<Temp>> ReservedIrRegisters::incomingFloatParameterLocations;

void ReservedIrRegisters::Initialize()
{
    StackPointer = std::make_shared<Temp>();
    FramePointer = std::make_shared<Temp>();

    RAX = std::make_shared<Temp>();
    RDI  = std::make_shared<Temp>();
    RSI = std::make_shared<Temp>();
    RDX  = std::make_shared<Temp>();
    RCX  = std::make_shared<Temp>();
    R8 = std::make_shared<Temp>();
    R9  = std::make_shared<Temp>();

    XMM0  = std::make_shared<Temp>();
    XMM1 = std::make_shared<Temp>();
    XMM2  = std::make_shared<Temp>();
    XMM3 = std::make_shared<Temp>();
    XMM4  = std::make_shared<Temp>();
    XMM5 = std::make_shared<Temp>();
    XMM6  = std::make_shared<Temp>();
    XMM7 = std::make_shared<Temp>();

    incomingIntegerParameterLocations = {RDI, RSI, RDX, RCX, R8, R9};
    incomingFloatParameterLocations = {XMM0, XMM1,XMM2,XMM3,XMM4,XMM5,XMM6,XMM7};
}