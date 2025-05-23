#pragma once
#include "ir_statement.hh"
#include "../../frame/label.hh"
#include <memory>

// Constant value of name N, like a label in assembly
class IRLabel : public IRStatement, public std::enable_shared_from_this<IRLabel>
{
        public:
        std::shared_ptr<Label> label;
        
        IRLabel(std::shared_ptr<Label> l);

        ~IRLabel() override = default;

        std::string ToDotFormat(int&) override;

                        
        std::shared_ptr<IRStatement> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids () override;
};