#pragma once
#include "assembly_instruction.hh"

class AssemblyOper : public AssemblyInstruction
{
    public:
        std::shared_ptr<TempList> dst;
        std::shared_ptr<TempList> src;

        std::shared_ptr<Targets> jump;

        AssemblyOper(std::string, std::shared_ptr<TempList>,std::shared_ptr<TempList>, std::shared_ptr<LabelList>);

        AssemblyOper(std::string, std::shared_ptr<TempList>,std::shared_ptr<TempList>);

        std::shared_ptr<TempList> Use() override;
        std::shared_ptr<TempList> Def() override;
        std::shared_ptr<Targets> Jumps() override;
};