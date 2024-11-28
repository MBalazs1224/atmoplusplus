#pragma once
#include <string>
#include "access.hh"
#include "temp.hh"
// Represents a temporary storage location (register)
class InReg : public Access
{  
    private:
        Temp temp;
    public:
        InReg();
        ~InReg() = default;
        std::string ToString();

};