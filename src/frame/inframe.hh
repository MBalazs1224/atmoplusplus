#pragma once
#include "access.hh"
#include "../ir/expressions/all_ir_expressions.hh"
#include <string>
#include <memory>
class InFrame : public Access
{
    public:
        // The offset from the frame pointer
        int offset;

        int size;

        InFrame(int, int);

        ~InFrame() = default;
        
        std::string ToString();

        std::shared_ptr<IRExpression> AsExpression(std::shared_ptr<IRExpression> framePointer) override;
        std::shared_ptr<IRExpression> AsExpressionNoDereference(std::shared_ptr<IRExpression> framePointer) override;
};