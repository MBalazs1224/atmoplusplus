#pragma once
#include "translate_expression.hh"

class TranslateConditionalExpression : TranslateExpression
{
    private:
        std::shared_ptr<IRExpression> exp;
    public:

        TranslateConditionalExpression(std::shared_ptr<IRExpression> exp_in);

        virtual std::shared_ptr<IRExpression> ToValueExpression() override;

        virtual std::shared_ptr<IRStatement> ToNoValueExpression() override;

        virtual std::shared_ptr<IRStatement> ToConditionExpression(std::shared_ptr<Label> trueLabel, std::shared_ptr<Label> falseLabel) override;
};