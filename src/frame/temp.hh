#pragma once
#include <string>

// Represents a temporary storage location (register)
class Temp
{
    private:
        static int count;
        int num;
    public:
        int sizeNeeded;
        Temp();
        Temp(int);
        ~Temp() = default;
        std::string ToString();
};