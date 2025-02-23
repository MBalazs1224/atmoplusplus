#include "offset_from_object.hh"

OffsetFromObject::OffsetFromObject(int offsetIn) : offset(offsetIn)
{}


std::shared_ptr<IRExpression> OffsetFromObject::AsExpression(std::shared_ptr<IRExpression> objectLocation)
{
    std::shared_ptr<IRExpression> correctOffset = objectLocation;

    // RDI will contain a pointer to class object inside class functions
    if(shouldUseRDI)
    {
        correctOffset = std::make_shared<IRTemp>(ReservedIrRegisters::RDI);
    }
    

    return std::make_shared<IRBinaryOperator>(
        BinaryOperator::PLUS,
        correctOffset,
        std::make_shared<IRConst>(offset)
    );
}