#pragma once
#include "translate_expression.hh"

class TranslateValueExpression : public TranslateExpression
{
    private:
        std::shared_ptr<IRExpression> exp;
    public:

        TranslateValueExpression(std::shared_ptr<IRExpression> exp_in);

        virtual std::shared_ptr<IRExpression> ToValueExpression() override;

        virtual std::shared_ptr<IRStatement> ToNoValueExpression() override;

        virtual std::shared_ptr<IRStatement> ToConditionExpression(std::shared_ptr<Label> trueLabel, std::shared_ptr<Label> flaseLabel) override ;
};