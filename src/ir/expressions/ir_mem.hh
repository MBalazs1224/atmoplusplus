#pragma once
#include <memory>
#include "ir_expression.hh"


// Represents wordSize bytes of memory starting at exp location
class IRMem : public IRExpression
{
    public:
    std::shared_ptr<IRExpression> exp;
        IRMem(std::shared_ptr<IRExpression>);

        std::string ToDotFormat(int &nodeCounter) override;
};