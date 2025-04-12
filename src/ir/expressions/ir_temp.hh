#pragma once
#include "ir_expression.hh"
#include "../../frame/temp.hh"
#include <memory>

// Similiar to a register, but we have unlimited of this
class IRTemp : public IRExpression, public std::enable_shared_from_this<IRTemp>
{
    public:
        std::shared_ptr<Temp> temp;
        IRTemp(std::shared_ptr<Temp>);

        std::string ToDotFormat(int&) override;


        std::shared_ptr<IRExpression> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids() override;
};
