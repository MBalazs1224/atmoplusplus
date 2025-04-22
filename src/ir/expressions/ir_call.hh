#pragma once
#include <memory>
#include "ir_expression.hh"
#include "ir_expression_list.hh"
#include "../../frame/label.hh"
#include "../../frame/datasize.hh"



// Represents a function call, func is evaluated before args who are evaluated left to right
class IRCall : public IRExpression
{
    public:
        std::shared_ptr<IRExpression> func;
        std::shared_ptr<IRExpressionList> args;

        std::shared_ptr<IRExpressionList> argumentLocations;
        // Indicates, wether the return value is needed later (in case of void functions and constructors it's not)
        bool returnValueNeeded = true;
        
        DataSize returnSize;



        IRCall(std::shared_ptr<IRExpression>, std::shared_ptr<IRExpressionList>, std::shared_ptr<IRExpressionList>, bool, DataSize);

        std::string ToDotFormat(int&) override;

        std::shared_ptr<IRExpression> Build (std::shared_ptr<IRExpressionList> kids)override;
        std::shared_ptr<IRExpressionList> Kids() override;

        virtual DataSize Size() override;

};