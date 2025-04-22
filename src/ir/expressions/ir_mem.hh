#pragma once
#include <memory>
#include "ir_expression.hh"
#include "ir_expression_list.hh"
#include "../../frame/datasize.hh"

// Represents wordSize bytes of memory starting at exp location
class IRMem : public IRExpression
{
    public:
        std::shared_ptr<IRExpression> exp;

        int bytesNeeded;

        IRMem(std::shared_ptr<IRExpression>, int);

        std::string ToDotFormat(int &nodeCounter) override;

        std::shared_ptr<IRExpression> Build (std::shared_ptr<IRExpressionList> kids) override;
        std::shared_ptr<IRExpressionList> Kids() override;

        virtual DataSize Size() override;

}; 