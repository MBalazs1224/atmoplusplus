#pragma once
#include "ir_expression.hh"

// Integer constant
class IRConst : public IRExpression, public std::enable_shared_from_this<IRConst>
{
    public:
        int value;
        IRConst(int);

        std::string ToDotFormat(int&) override;


        std::shared_ptr<IRExpression> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids() override;
};