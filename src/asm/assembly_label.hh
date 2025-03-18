#pragma once
#include "assembly_instruction.hh"
#include "../frame/label.hh"
class AssemblyLabel : public AssemblyInstruction
{
    public:
        std::shared_ptr<Label> label;

        AssemblyLabel(std::string, std::shared_ptr<Label>);

        std::shared_ptr<TempList> Use() override;
        std::shared_ptr<TempList> Def() override;
        std::shared_ptr<Targets> Jumps() override;
};