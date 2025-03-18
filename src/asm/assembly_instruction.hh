#pragma once
#include <memory>
#include <string>
#include "../frame/templist.hh"
#include "targets.hh"
class AssemblyInstruction
{
    public:
        std::string assemblyInstruction;
        virtual std::shared_ptr<TempList> Def() = 0;
        virtual std::shared_ptr<TempList> Use() = 0;

        virtual std::shared_ptr<Targets> Jumps() = 0;
    };