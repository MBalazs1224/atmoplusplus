#include "array_subscript.hh"

ArraySubscriptExpression::ArraySubscriptExpression(std::shared_ptr<IExpressionable> left_in, std::shared_ptr<IExpressionable> right_in,yy::location loc) : TwoOperandExpression(left_in,right_in,loc)
{
}

std::shared_ptr<Type> ArraySubscriptExpression::GetType()
{
    // At this point the Check funtion checked that the left expression is an array so we can just cast it

    auto array = std::dynamic_pointer_cast<Array>(left->GetType());

    return array->inner_type;
}

bool ArraySubscriptExpression::Check()
{
    if(!left->Check())
    {
        return false;
    }

    if(!right->Check())
    {
        return false;
    }

    auto arrayType = left->GetType();
    if (!std::dynamic_pointer_cast<Array>(arrayType))
    {
        Error::ShowError(Helper::FormatString("Only array types can appear on the left side of array subscript expression! (received '%s')",arrayType->ToString().c_str()),left->location);
        return false;
    }

    auto intType = right->GetType();
    if (!std::dynamic_pointer_cast<TypeInteger>(intType))
    {
        Error::ShowError(Helper::FormatString("Only integer types can appear on the right side of array subscript expression! (received '%s')",intType->ToString().c_str()),right->location);
        return false;
    }

    return true;
}

std::shared_ptr<TranslateExpression> ArraySubscriptExpression::TranslateExpressionToIr()
{
    // (array_address) + (data_size * index) is the algorithm used to index into an array

    auto indexExpression = right->TranslateExpressionToIr()->ToValueExpression();

    // We need to add 1 to the wanted index because the first element will be the size of the array so everything is pushed to the side by 1

    //FIXME: This might still need to be pushed by 4 because the first element is always an integer (the size) so the first actual element always starts at 5

    auto correctedIndex = std::make_shared<IRBinaryOperator>(
        BinaryOperator::PLUS,
        indexExpression,
        std::make_shared<IRConst>(1)
    );

    auto locationOfArray = left->TranslateExpressionToIr()->ToValueExpression();

    // The semantic analyzer already checked that the left expression is of array type

    auto leftAsArray = std::dynamic_pointer_cast<Array>(left->GetType());


    int sizeOfElementsInTheArray = leftAsArray->inner_type->GetSize();

    // (data_size * index)
    auto multipliedIndexValue = std::make_shared<IRBinaryOperator>(
        BinaryOperator::MULTIPLY,
        std::make_shared<IRConst>(sizeOfElementsInTheArray),
        correctedIndex
    );


    auto offsetIntoArray = std::make_shared<IRBinaryOperator>(
        BinaryOperator::MINUS,
        locationOfArray,
        multipliedIndexValue
    );

    auto memExpression = std::make_shared<IRMem>(
        offsetIntoArray
    );


    return std::make_shared<TranslateValueExpression>(memExpression);



}

std::shared_ptr<IRStatement> ArraySubscriptExpression::TranslateToIR()
{
    //TODO: "Implement array subscript to ir"
    return nullptr;
}