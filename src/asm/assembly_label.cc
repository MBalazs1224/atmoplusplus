#include "assembly_label.hh"

AssemblyLabel::AssemblyLabel(std::string asmIn, std::shared_ptr<Label> l)
{
    this->assemblyInstruction = asmIn;

    this->label = l;
}

std::shared_ptr<TempList> AssemblyLabel::Use()
{
    return nullptr;
}
std::shared_ptr<TempList> AssemblyLabel::Def()
{
    return nullptr;
}
std::shared_ptr<Targets> AssemblyLabel::Jumps()
{
    return nullptr;
}