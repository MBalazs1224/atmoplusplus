#include "temp.hh"

int Temp::count = 0;

Temp::Temp() : Temp(8) // Default to 64 bit
{
    
}

Temp::Temp(int size) : sizeNeeded(size)
{
    num = count++;
}

std::string Temp::ToString()
{
    return "t" + std::to_string(num);
}