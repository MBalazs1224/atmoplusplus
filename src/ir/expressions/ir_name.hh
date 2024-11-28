#pragma once
#include "ir_expression.hh"
#include "../../frame/label.hh"

// Represents an assembly label
class IRName : IRExpression
{
        Label label;
    public:
        IRName(Label);
};