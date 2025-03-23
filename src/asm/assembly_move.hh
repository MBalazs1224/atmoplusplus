#pragma once
#include "assembly_instruction.hh"

class AssemblyMove : public AssemblyInstruction
{
    public:
        std::shared_ptr<Temp> dst;
        std::shared_ptr<Temp> src;

        AssemblyMove(std::string, std::shared_ptr<Temp> d, std::shared_ptr<Temp> s);

        std::shared_ptr<TempList> Use() override;
        std::shared_ptr<TempList> Def() override;
        std::shared_ptr<Targets> Jumps() override;
};