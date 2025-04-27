#pragma once
#include "access.hh"
#include "../ir/expressions/all_ir_expressions.hh"

// Represents that the resource can be found offsetted from an object's vtable pointer
class OffsetFromVtable : public Access
{
    private:
        int offset;
    public:
        std::shared_ptr<IRExpression> AsExpression(std::shared_ptr<IRExpression> objectLocation) override;

        std::shared_ptr<IRExpression> AsExpressionNoDereference(std::shared_ptr<IRExpression> objectLocation) override;

        OffsetFromVtable(int);
};