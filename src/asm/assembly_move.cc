#include "assembly_move.hh"

AssemblyMove::AssemblyMove(std::string asmIn, std::shared_ptr<Temp> d , std::shared_ptr<Temp> s)
{
    this->assemblyInstruction = asmIn;
    this->dst = d;
    this->src = s;
}


std::shared_ptr<TempList> AssemblyMove::Use()
{
    return std::make_shared<TempList>(
        src,
        nullptr
    );
}

std::shared_ptr<TempList> AssemblyMove::Def()
{
    return std::make_shared<TempList>(
        dst,
        nullptr
    );
}

std::shared_ptr<Targets> AssemblyMove::Jumps()
{
    return nullptr;
}
