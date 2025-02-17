#include "offset_from_object.hh"

OffsetFromObject::OffsetFromObject(int offsetIn) : offset(offsetIn)
{}


std::shared_ptr<IRExpression> OffsetFromObject::AsExpression(std::shared_ptr<IRExpression> objectLocation)
{
    return std::make_shared<IRBinaryOperator>(
        BinaryOperator::PLUS,
        objectLocation,
        std::make_shared<IRConst>(offset)
    );
}