#pragma once
#include "ir_expression.hh"
#include "../../frame/label.hh"
#include <memory>

// Represents an assembly label
class IRName : public IRExpression, public std::enable_shared_from_this<IRName>
{
    public:
        std::shared_ptr<Label> label;
        IRName(std::shared_ptr<Label>);

    std::string ToDotFormat(int&) override;


    std::shared_ptr<IRExpression> Build (std::shared_ptr<IRExpressionList> kids) override;
    std::shared_ptr<IRExpressionList> Kids() override;

    virtual DataSize Size() override;

};