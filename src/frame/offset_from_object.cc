#include "offset_from_object.hh"

OffsetFromObject::OffsetFromObject(int offsetIn, int sizeIn) : offset(offsetIn), size(sizeIn)
{}


std::shared_ptr<IRExpression> OffsetFromObject::AsExpression(std::shared_ptr<IRExpression> objectLocation)
{
    std::shared_ptr<IRExpression> correctOffset = objectLocation;

    // RDI will contain a pointer to class object inside class functions
    if(shouldUseRDI)
    {
        correctOffset = std::make_shared<IRTemp>(ReservedIrRegisters::RDI);
    }
    
    
    // We need to dereference that memory location
    return std::make_shared<IRMem>(
        std::make_shared<IRBinaryOperator>(
            BinaryOperator::PLUS,
            correctOffset,
            std::make_shared<IRConst>(offset)
        ),
        this->size
    );

}

std::shared_ptr<IRExpression> OffsetFromObject::AsExpressionNoDereference(std::shared_ptr<IRExpression> objectLocation)
{
    std::shared_ptr<IRExpression> correctOffset = objectLocation;

    // RDI will contain a pointer to class object inside class functions
    if(shouldUseRDI)
    {
        correctOffset = std::make_shared<IRTemp>(ReservedIrRegisters::RDI);
    }
    
    
    // Don't dereference that memory location
    return std::make_shared<IRBinaryOperator>(
        BinaryOperator::PLUS,
        correctOffset,
        std::make_shared<IRConst>(offset)
    );

}