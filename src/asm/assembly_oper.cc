#include "assembly_oper.hh"

std::shared_ptr<TempList> AssemblyOper::Use()
{
    return src;
}

std::shared_ptr<TempList> AssemblyOper::Def()
{
    return dst;
}

AssemblyOper::AssemblyOper(std::string asmIn, std::shared_ptr<TempList> d,std::shared_ptr<TempList> s, std::shared_ptr<LabelList> j)
{
    this->assemblyInstruction = asmIn;
    this->dst = d;
    this->src = s;
    this->jump = std::make_shared<Targets>(j);
}

AssemblyOper::AssemblyOper(std::string asmIn, std::shared_ptr<TempList> d,std::shared_ptr<TempList> s) : AssemblyOper(asmIn,d,s, nullptr)
{
}


std::shared_ptr<Targets> AssemblyOper::Jumps()
{
    return jump;
}
