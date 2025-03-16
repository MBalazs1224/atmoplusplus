#pragma once
#include "ir_expression.hh"
#include "../../frame/label.hh"
#include <memory>

// Represents an assembly label
class IRName : public IRExpression
{
    public:
        std::shared_ptr<Label> label;
        IRName(std::shared_ptr<Label>);

    std::string ToDotFormat(int&) override;
};