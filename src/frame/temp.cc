#include "temp.hh"

int Temp::count = 0;

Temp::Temp()
{
    num = count++;
}

std::string Temp::ToString()
{
    return "t" + std::to_string(num);
}