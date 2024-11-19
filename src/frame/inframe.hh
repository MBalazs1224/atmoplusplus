#pragma once
#include "access.hh"
#include <string>
class InFrame : public Access
{
    public:
        // The offset from the frame pointer
        int offset;

        InFrame(int);

        ~InFrame() = default;
        
        std::string ToString();
};