#pragma once
#include "ir_statement.hh"

// Will indicate the the enter instruction needs to be printed
class IREnter : public IRStatement
{
    public:
        int neededBytes;
        IREnter(int);
        std::string ToDotFormat(int&) override;
};