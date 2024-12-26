#pragma once
#include "ir_expression.hh"
#include "../../frame/label.hh"
#include <memory>

// Represents an assembly label
class IRName : public IRExpression
{
        std::shared_ptr<Label> label;
    public:
        IRName(std::shared_ptr<Label>);
};