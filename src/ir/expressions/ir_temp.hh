#pragma once
#include "ir_expression.hh"
#include "../../frame/temp.hh"

// Similiar to a register, but we have unlimited of this
class IRTemp : public IRExpression
{
        Temp temp;
    public:
        IRTemp(Temp);
};