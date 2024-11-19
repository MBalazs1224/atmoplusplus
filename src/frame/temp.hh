#pragma once
#include <string>

// Represents a temporary storage location (register)
class Temp
{
    private:
        static int count;
        int num;
    public:
        Temp();
        ~Temp() = default;
        std::string ToString();
};