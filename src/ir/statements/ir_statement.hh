#pragma once 
#include "../ir_node.hh"

class IRStatement : public IRNode
{
    public:
        virtual ~IRStatement() = 0;
};