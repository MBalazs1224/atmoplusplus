#pragma once
#include "ir_statement.hh"
#include "../../frame/label.hh"

// Constant value of name N, like a label in assembly
class IRLabel : IRStatement
{
        //TODO: Create copy constructor for Label
        Label label;
    public:
        
        IRLabel(Label l);
};