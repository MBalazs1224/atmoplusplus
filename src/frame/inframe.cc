#include "inframe.hh"

InFrame::InFrame(int offset_in) : offset(offset_in)
{
}

std::string InFrame::ToString()
{
    return std::to_string(offset) + "(fp)";
}