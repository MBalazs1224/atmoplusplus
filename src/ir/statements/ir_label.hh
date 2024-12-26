#pragma once
#include "ir_statement.hh"
#include "../../frame/label.hh"
#include <memory>

// Constant value of name N, like a label in assembly
class IRLabel : public IRStatement
{
        //TODO: Create copy constructor for Label
        std::shared_ptr<Label> label;
    public:
        
        IRLabel(std::shared_ptr<Label> l);
};