#include "less_than_expression.hh"

LessThanExpression::LessThanExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in, yy::location loc)
    : TwoOperandExpression(left_in,right_in,loc) {}

std::shared_ptr<Type> LessThanExpression::GetType()
{
    return Helper::BooleanType;
}

bool LessThanExpression::Check() {
    if (!left->Check() || !right->Check())
    {
        return false;
    }
    
    auto left_type = left->GetType();
    auto right_type = right->GetType();
    
    if (left_type->NotCompatible(right_type))
    {
        Error::ShowError(Helper::FormatString(
            "The two operands of LESS THAN (<) expression must be of same type! (received '%s' and '%s')",
            left_type->ToString().c_str(),
            right_type->ToString().c_str()
        ), location);
        return false;
    }

    if(left_type->NotCompatible(Helper::IntegerType) && left_type->NotCompatible(Helper::FloatType))
    {
        Error::ShowError("Only numerical values can be compared with LESS THAN (<) operator!",this->location);
        return false;
    }

    return true;
}

std::shared_ptr<TranslateExpression> LessThanExpression::TranslateExpressionToIr()
{
        // Create the labels

    auto trueLabel = std::make_shared<Label>();
    auto falseLabel = std::make_shared<Label>();
    auto joinLabel = std::make_shared<Label>();

    // Create the register that will be used to store the final value

    auto reg = std::make_shared<Temp>(DataSize::DWord);

    // Translate the left and right expressions into IR value expression, because their values will be compared

    auto leftExp = left->TranslateExpressionToIr()->ToValueExpression();
    auto rightExp = right->TranslateExpressionToIr()->ToValueExpression();

    // Create the comparison expression

    auto comparisonExpression = std::make_shared<IRCJump>(
        RelationalOperator::LessThan,
        leftExp,
        rightExp,
        trueLabel,
        falseLabel
    );

    // Create true path

    auto trueSequence = std::make_shared<IRSequence>(
        std::make_shared<IRLabel>(trueLabel),
        std::make_shared<IRSequence>(
            std::make_shared<IRMove>(
                std::make_shared<IRTemp>(reg),
                std::make_shared<IRConst>(1)
            ),
            std::make_shared<IRJump>(std::make_shared<IRName>(joinLabel))
        )
    );

    // Create false path

    auto falseSequence = std::make_shared<IRSequence>(
        std::make_shared<IRLabel>(falseLabel),
        std::make_shared<IRSequence>(
            std::make_shared<IRMove>(
                std::make_shared<IRTemp>(reg),
                std::make_shared<IRConst>(0)
            ),
            std::make_shared<IRLabel>(joinLabel)
        )
    );

    auto finalSequence = std::make_shared<IRSequence>(
        comparisonExpression,
        std::make_shared<IRSequence>(
            trueSequence,
            falseSequence
        )
    );

    auto finalEseq = std::make_shared<IREseq>(std::make_shared<IRTemp>(reg), finalSequence);

    return std::make_shared<TranslateValueExpression>(finalEseq);
}

std::shared_ptr<IRStatement> LessThanExpression::TranslateToIR()
{
    throw std::logic_error("LessThanExpression::TranslateToIR() should not be called!");
}

