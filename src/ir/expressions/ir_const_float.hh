#pragma once

#include "ir_expression.hh"

// Integer constant
class IRConstFloat : public IRExpression, public std::enable_shared_from_this<IRConstFloat>
{
    public:
        double value;
        IRConstFloat(double);

        std::string ToDotFormat(int&) override;

        std::shared_ptr<IRExpression> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids() override;

        virtual DataSize Size() override;

};