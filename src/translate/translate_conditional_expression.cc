#include "translate_conditional_expression.hh"
#include <stdexcept>
TranslateConditionalExpression::TranslateConditionalExpression(std::shared_ptr<IRExpression> exp_in) : exp(std::move(exp_in))
{
}

std::shared_ptr<IRExpression> TranslateConditionalExpression::ToValueExpression()
{
    // The reg that will hold the result of the expression
    auto reg = std::make_shared<Temp>(); 

    // The true and false labels
    auto trueLabel = std::make_shared<Label>();
    auto falseLabel = std::make_shared<Label>();

    // Instructions to move 0 into a register (this will be the value if the condition is false)

    auto move0IntoReg = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(reg), std::make_shared<IRConst>(0));

    // Execute the move0toReg instruction , then immediately create the true label

    auto move0ThenTrueLabel= std::make_shared<IRSequence>(
        move0IntoReg,
        std::make_shared<IRLabel>(trueLabel));

    // Create the false label, then immediately execute the move0ThenTrueLabel sequence

    auto labelFalseThenMove0 = std::make_shared<IRSequence>(
        std::make_shared<IRLabel>(falseLabel),
        move0ThenTrueLabel);

    // Translate the expression to a condition expression (so we can decide if the want to jump to the true or false label)

    auto conditionStmt = ToConditionExpression(trueLabel, falseLabel);

    // Create a sequence that executes the conditionStmt, then immediately executes the labelFalseThenMove0 sequence

    auto conditionThenFalse = std::make_shared<IRSequence>(
        conditionStmt,
        labelFalseThenMove0);

    // Move 1 to the temp register if the condition is true

    auto move1IntoReg = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(reg),
        std::make_shared<IRConst>(1));

    // Create a sequence that executes the conditionThenFalse sequence, then immediately executes the move1IntoReg instruction

    auto conditionThenFalseThenMove1 = std::make_shared<IRSequence>(
        conditionThenFalse,
        move1IntoReg);

    // Executes all the instructions above, then gets the value of the register.

    auto finalIR = std::make_shared<IREseq>(
        std::make_shared<IRTemp>(reg),
        conditionThenFalseThenMove1 
    );

    return finalIR;
}

std::shared_ptr<IRStatement> TranslateConditionalExpression::ToNoValueExpression()
{
    return std::make_shared<IREvaluateExpression>(exp);
}

std::shared_ptr<IRStatement> TranslateConditionalExpression::ToConditionExpression(std::shared_ptr<Label> trueLabel, std::shared_ptr<Label> falseLabel)
{
    // Need to check if the expression equals to 1, if it does jump to the true label, otherwise jump to false

    auto oneExpression = std::make_shared<IRConst>(1);

    auto condition = std::make_shared<IRCJump>(RelationalOperator::EQUALS, exp, oneExpression, trueLabel, falseLabel);

    return condition;
}