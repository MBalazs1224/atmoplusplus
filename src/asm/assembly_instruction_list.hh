#pragma once
#include "assembly_instruction.hh"
#include <memory>

class AssemblyInstructionList
{
    public:
        std::shared_ptr<AssemblyInstruction> head;
        std::shared_ptr<AssemblyInstructionList> tail;

        AssemblyInstructionList(std::shared_ptr<AssemblyInstruction> headIn, std::shared_ptr<AssemblyInstructionList> tailIn) : head(headIn), tail(tailIn) {}

        AssemblyInstructionList() : head(nullptr), tail(nullptr) {}
};