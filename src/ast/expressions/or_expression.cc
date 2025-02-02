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

    // Label for the isntructions of moving 0 into the reg
    auto move0Label = std::make_shared<Label>();


    // Translate the left and right expressions into IR

    auto leftExp = left->TranslateExpressionToIr()->ToConditionExpression(trueLabel, falseLabel);

    auto rightExp = right->TranslateExpressionToIr()->ToConditionExpression(trueLabel, move0Label);

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


    auto printMove0Label = std::make_shared<IRLabel>(
        move0Label
    );

    auto printLabelAndMove0 = std::make_shared<IRSequence>(
        printMove0Label,
        move0IntoReg
    );

    // If the first expression is true, set the reg to 1 and jump to the join label because of short circuiting

    auto trueSequence = std::make_shared<IRSequence>(
        std::make_shared<IRLabel>(trueLabel),
        std::make_shared<IRSequence>(
            move1IntoReg,
            std::make_shared<IRJump>(std::make_shared<IRName>(joinLabel))
        )
    );


    // If the first expression is false, check the second expression


    auto falseSequence = std::make_shared<IRSequence>(
        std::make_shared<IRLabel>(falseLabel),
        std::make_shared<IRSequence>(
            rightExp,
            printLabelAndMove0
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
    auto finalSeq = std::make_shared<IREseq>(std::make_shared<IRTemp>(reg), finalSequence);

    return std::make_shared<TranslateValueExpression>(finalSeq);
    
}

std::shared_ptr<IRStatement> OrExpression::TranslateToIR()
{
    throw std::logic_error("OrExpression::TranslateToIR() should not be called!");
}
