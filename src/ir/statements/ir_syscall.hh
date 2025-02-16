#pragma once
#include "ir_statement.hh"

// Indicates that the system call instruction is needed
class IRSysCall : public IRStatement
{
    public:
        std::string ToDotFormat(int&) override;
};