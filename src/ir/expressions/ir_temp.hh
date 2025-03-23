#pragma once
#include "ir_expression.hh"
#include "../../frame/temp.hh"
#include <memory>

// Similiar to a register, but we have unlimited of this
class IRTemp : public IRExpression
{
    public:
        std::shared_ptr<Temp> temp;
        IRTemp(std::shared_ptr<Temp>);

        std::string ToDotFormat(int&) override;
};