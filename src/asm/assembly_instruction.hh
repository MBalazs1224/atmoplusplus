#pragma once
#include <memory>
#include <string>
#include "../frame/templist.hh"
#include "targets.hh"
#include "../codegen/tempmap.hh"
#include <sstream>
class AssemblyInstruction
{
    public:
        std::string assemblyInstruction;
        virtual std::shared_ptr<TempList> Def() = 0;
        virtual std::shared_ptr<TempList> Use() = 0;

        std::shared_ptr<Temp> NthTemp(std::shared_ptr<TempList>, int );
        std::shared_ptr<Label> NthLabel(std::shared_ptr<LabelList>, int );

        virtual std::shared_ptr<Targets> Jumps() = 0;

        std::string Format(std::shared_ptr<TempMap> map);
    };