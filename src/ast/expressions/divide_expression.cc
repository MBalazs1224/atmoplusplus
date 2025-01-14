#include "divide_expression.hh"
bool DivideExpression::Check()
{
    if(!left->Check() || !right->Check())
    {
        return false;
    }
    auto left_type = left->GetType();
    auto right_type = right->GetType();
    if (left_type->NotCompatible(right_type))
    {
        Error::ShowError(Helper::FormatString("The two operands of DIVIDE (/) expression must be of same type! (received '%s' and '%s')",left_type->ToString().c_str(),right_type->ToString().c_str()),location);
        return false;
    }
    return true;
}

DivideExpression::DivideExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : TwoOperandExpression(left_in,right_in,loc)
{
}

std::shared_ptr<TranslateExpression> DivideExpression::TranslateExpressionToIr()
{
    auto leftExpression = left->TranslateExpressionToIr()->ToValueExpression();
    auto rightExpression = left->TranslateExpressionToIr()->ToValueExpression();

    auto divideExpression = std::make_shared<IRBinaryOperator>(BinaryOperator::DIVIDE, leftExpression, rightExpression);

    return std::make_shared<TranslateValueExpression>(divideExpression);
}

std::shared_ptr<IRStatement> DivideExpression::TranslateToIR()
{
    throw std::runtime_error("DivideExpression should not be translated to IR as nodes");
}


std::shared_ptr<Type> DivideExpression::GetType()
{
    return Helper::FloatType;
}