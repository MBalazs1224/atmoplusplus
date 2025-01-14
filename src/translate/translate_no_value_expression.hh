#pragma once
#include "translate_expression.hh"


// For expressions that don't have a value (like function calls, assignments, etc)

class TranslateNoValueExpression : public TranslateExpression
{
    private:
        std::shared_ptr<IRStatement> stmt;
    public:

        TranslateNoValueExpression(std::shared_ptr<IRStatement> stmt_in);

        virtual std::shared_ptr<IRExpression> ToValueExpression() override;

        virtual std::shared_ptr<IRStatement> ToNoValueExpression() override;

        virtual std::shared_ptr<IRStatement> ToConditionExpression(std::shared_ptr<Label> trueLabel, std::shared_ptr<Label> flaseLabel) override ;
};