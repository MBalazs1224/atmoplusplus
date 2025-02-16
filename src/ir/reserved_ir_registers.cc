#include "reserved_ir_registers.hh"

std::shared_ptr<Temp> ReservedIrRegisters::StackPointer;
std::shared_ptr<Temp> ReservedIrRegisters::FramePointer;
std::shared_ptr<Temp> ReservedIrRegisters::RAX;
std::shared_ptr<Temp> ReservedIrRegisters::RDI;

void ReservedIrRegisters::Initialize()
{
    StackPointer = std::make_shared<Temp>();
    FramePointer = std::make_shared<Temp>();
    RAX = std::make_shared<Temp>();
    RDI  = std::make_shared<Temp>();
}