#include "or_expression.hh"

OrExpression::OrExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in, yy::location loc)
    : TwoOperandExpression(left_in,right_in,loc) {}

std::shared_ptr<Type> OrExpression::GetType()
{
    return Helper::BooleanType;
}

bool OrExpression::Check() {
    if (!left->Check() || !right->Check()) 
    {
        return false;
    }

    auto boolean = Helper::BooleanType;
    auto left_type = left->GetType();
    auto right_type = right->GetType();

    if (left_type->NotCompatible(boolean) || right_type->NotCompatible(boolean)) 
    {
        Error::ShowError(Helper::FormatString(
            "Both of the two operands of OR (||) expression must be of type boolean! (received '%s' and '%s')",
            left_type->ToString().c_str(),
            right_type->ToString().c_str()
        ), location);
        return false;
    }

    return true;
}

std::shared_ptr<TranslateExpression> OrExpression::TranslateExpressionToIr()
{
    // Create the labels
    auto trueLabel = std::make_shared<Label>();
    auto falseLabel = std::make_shared<Label>();
    auto joinLabel = std::make_shared<Label>();

    // Translate the left and right expressions into IR

    auto leftExp = left->TranslateExpressionToIr()->ToConditionExpression(trueLabel, falseLabel);

    auto rightExp = right->TranslateExpressionToIr()->ToConditionExpression(trueLabel, falseLabel);

    // Create the register that will be used to store the final value

    auto reg = std::make_shared<Temp>();

    // Statement to move 1 into the register
    
    auto move1IntoReg = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(reg),
        std::make_shared<IRConst>(1)
    );

    // Statement to move 0 into the register

    auto move0IntoReg = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(reg),
        std::make_shared<IRConst>(0)
    );

    // If the first expression is true, set the reg to 1 and jump to the join label because of short circuiting

    auto trueSequence = std::make_shared<IRSequence>(
        std::make_shared<IRLabel>(trueLabel),
        move1IntoReg,
        std::make_shared<IRJump>(joinLabel)
    );


    // If the first expression is false, check the second expression

    // FIXME: The false label might need to be moved after the rightexp or the tight exp might not need to jump tp the false label because it might create an infinite loop

    auto falseSequence = std::make_shared<IRSequence>(
        std::make_shared<IRLabel>(falseLabel),
        std::make_shared<IRSequence>(
            rightExp,
            std::make_shared<IRSequence>(
                move0IntoReg,
                std::make_shared<IRJump>(joinLabel)
            )
        )
    );

    // Combine the true and false paths

    auto finalSequence = std::make_shared<IRSequence>(
        leftExp,
        std::make_shared<IRSequence>(
            trueSequence,
            std::make_shared<IRSequence>(
                falseSequence,
                std::make_shared<IRLabel>(joinLabel)
            )
        )
    );

    // Execute the final sequence and get the reg as the final value
    auto finalSeq = std::make_shared<IREseq>(finalSequence, reg);

    return std::make_shared<TranslateExpression>(finalSeq);
    
}

std::shared_ptr<IRStatement> OrExpression::TranslateToIR()
{
    throw std::logic_error("OrExpression::TranslateToIR() should not be called!");
}
