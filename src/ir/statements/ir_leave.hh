#pragma once
#include "ir_statement.hh"

// Will indicate the the leave instruction needs to be printed
class IRLeave : public IRStatement
{
    public:
        std::string ToDotFormat(int&) override;
};