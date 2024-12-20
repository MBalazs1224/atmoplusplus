#include "add_expression.hh"

bool AddExpression::Check()
{
    if(!left->Check() || !right->Check())
    {
        return false;
    }
    auto left_type = left->GetType();
    auto right_type = right->GetType();
    if (left_type->NotCompatible(right_type))
    {
        Error::ShowError(Helper::FormatString("The two operands of ADD (+) expression must be of same type! (received '%s' and '%s')",left_type->ToString().c_str(),right_type->ToString().c_str()),location);
        return false;
    }
    
    return true;
}

AddExpression::AddExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : TwoOperandExpression(left_in,right_in,loc)
{
}

std::shared_ptr<Type> AddExpression::GetType()
{
    return left->GetType();
}

std::shared_ptr<TranslateExpression> AddExpression::TranslateExpressionToIr()
{
    // Generate both sides into an IR expression
    auto leftIR = left->TranslateExpressionToIr()->ToValueExpression(); 
    auto rightIR = right->TranslateExpressionToIr()->ToValueExpression();

    // Create the binary plus from the left and right
    auto addExpression = std::make_shared<IRBinaryOperator>(BinaryOperator::PLUS,leftIR,rightIR);

    // Wrap it in a translate to value epxression, so other expressions can use it
    return std::make_shared<TranslateValueExpression>(addExpression);
}