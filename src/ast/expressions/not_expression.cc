#include "not_expression.hh"

NotExpression::NotExpression(std::shared_ptr<IExpressionable> exp_in, yy::location loc)
    : OneOperandExpression(exp_in,loc) {}

std::shared_ptr<Type> NotExpression::GetType()
{
    return Helper::BooleanType;
}

bool NotExpression::Check()
{
    if (!expression->Check()) 
    {
        return false;
    }

    auto exp_type = expression->GetType();

    if (exp_type->NotCompatible(Helper::BooleanType)) 
    {
        Error::ShowError(Helper::FormatString(
            "The expression of not expression must be of type boolean! (received '%s')",
            exp_type->ToString().c_str()
        ), expression->location);
        return false;
    }

    return true;
}

std::shared_ptr<TranslateExpression> NotExpression::TranslateExpressionToIr()
{

    // To negate a boolean expression, we need to subtract it from 1 (1 - 1 = 0, 1 - 0 = 1)

    auto exp = expression->TranslateExpressionToIr()->ToValueExpression();

    // Create a register for the final value

    auto reg = std::make_shared<Temp>(DataSize::DWord);

    // Subtract the value from 1

    auto subtraction = std::make_shared<IRBinaryOperator>(
        BinaryOperator::MINUS,
        std::make_shared<IRConst>(1),
        exp
    );

    // Move the result into the register

    auto move = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(reg),
        subtraction
    );

    // Evaluate the expression and get the register for the value

    auto eseq = std::make_shared<IREseq>(
        std::make_shared<IRTemp>(reg),
        move
    );

    // Return the expressiion as a value expression

    return std::make_shared<TranslateValueExpression>(eseq);

    
}

std::shared_ptr<IRStatement> NotExpression::TranslateToIR()
{
    throw std::logic_error("NotExpression::TranslateToIR() should not be called");
}
