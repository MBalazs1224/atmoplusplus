#include "and_expression.hh"


bool AndExpression::Check()
{
    if(!left->Check() || !right->Check())
    {
        return false;
    }
    auto boolean = Helper::BooleanType;
    auto left_type = left->GetType();
    auto right_type = right->GetType();
    if (left_type->NotCompatible(boolean) || right_type->NotCompatible(boolean))
    {
        Error::ShowError(Helper::FormatString("Both of the two operands of AND (&&) expression must be of type boolean! (received '%s' and '%s')",left_type->ToString().c_str(),right_type->ToString().c_str()),location);
        return false;
    }
    return true;
}

std::shared_ptr<Type> AndExpression::GetType()
{
    return Helper::BooleanType;
}

AndExpression::AndExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : TwoOperandExpression(left_in,right_in,loc)
{
}

std::shared_ptr<TranslateExpression> AndExpression::TranslateExpressionToIr()
{
    // Logical operators need to be translated as value expressions, because their values would regardless be evaluated inside a condition and be compared to either 0 or 1

    // Need to put the end value inside a register

    auto reg = std::make_shared<Temp>();

    // The true, false and joining labels

    auto trueLabel = std::make_shared<Label>();
    auto falseLabel = std::make_shared<Label>();
    auto joinLabel = std::make_shared<Label>();

    auto move1IntoRegLabel = std::make_shared<Label>();
    // Translate left and right expressions into IR

    auto leftExp = left->TranslateExpressionToIr()->ToConditionExpression(trueLabel,falseLabel);

    auto rightExp = right->TranslateExpressionToIr()->ToConditionExpression(move1IntoRegLabel,falseLabel);

    // Statements to move 0 and 1 into the end register

    auto move0IntoReg = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(reg),
        std::make_shared<IRConst>(0)
    );

    auto move1IntoReg = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(reg),
        std::make_shared<IRConst>(1)
    );

    auto printMove1LabelAndExecuteMove = std::make_shared<IRSequence>(
        std::make_shared<IRLabel>(move1IntoRegLabel),
        move1IntoReg
    );

    // The sequence for the true path

    auto trueSequence = std::make_shared<IRSequence>(
        leftExp,
        std::make_shared<IRSequence>(
            std::make_shared<IRLabel>(trueLabel),
            rightExp
        )
    );

    // The sequence for the false path

    auto falseSequence = std::make_shared<IRSequence>(
        std::make_shared<IRLabel>(falseLabel),
        std::make_shared<IRSequence>(
            move0IntoReg,
            std::make_shared<IRJump>(std::make_shared<IRName>(joinLabel))
        )
    );

    // Combine the true and false paths

    auto finalSequence = std::make_shared<IRSequence>(
        trueSequence,
        std::make_shared<IRSequence>(
            falseSequence,
            std::make_shared<IRSequence>(
                printMove1LabelAndExecuteMove,
                std::make_shared<IRLabel>(joinLabel)
            )
            
        )
    );

    // Evaluate the final sequence and get the result inside the end register

    auto finalEseq = std::make_shared<IREseq>(
        std::make_shared<IRTemp>(reg),
        finalSequence
    );





    return std::make_shared<TranslateValueExpression>(finalEseq);
}

std::shared_ptr<IRStatement> AndExpression::TranslateToIR()
{
    
    throw std::runtime_error("AndExpression should not be translated to IR as nodes");
}


AndExpression::~AndExpression() = default;