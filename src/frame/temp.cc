#include "temp.hh"

int Temp::count = 0;

Temp::Temp() : Temp(DataSize::QWord) // Default to 64 bit
{
    
}

Temp::Temp(DataSize size) : sizeNeeded(size)
{
    num = count++;
}

std::string Temp::ToString()
{
    return "t" + std::to_string(num);
}

Temp::Temp(int num, DataSize size)
{
    this->num = num;
    this->sizeNeeded = size;
}

std::shared_ptr<Temp> Temp::Clone(DataSize wantedSize)
{
    auto newTemp = std::shared_ptr<Temp>(new Temp(this->num, wantedSize));
    return newTemp;
}