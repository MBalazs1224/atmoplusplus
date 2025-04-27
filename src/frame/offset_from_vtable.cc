#include "offset_from_vtable.hh"

OffsetFromVtable::OffsetFromVtable(int offsetIn) : offset(offsetIn)
{}


std::shared_ptr<IRExpression> OffsetFromVtable::AsExpression(std::shared_ptr<IRExpression> objectLocation)
{
    // It's the same as NoDereference but the final value dereferenced
    return std::make_shared<IRMem>(
        std::make_shared<IRMem>(
            this->AsExpressionNoDereference(objectLocation),
            DataSize::QWord //FIXME: DEfault to 64 bit pointer
        ),
        DataSize::QWord
    );
    

}

std::shared_ptr<IRExpression> OffsetFromVtable::AsExpressionNoDereference(std::shared_ptr<IRExpression> objectLocation)
{
     // Need to dereference the object and then it's vtable

     auto dereferenceObjectLocation = std::make_shared<IRMem>(
        objectLocation,
        DataSize::QWord
    );

    // The vtable pointer is a pointer starting from the object's memory address
    auto getVtablePointer = std::make_shared<IRMem>(
        dereferenceObjectLocation,
        DataSize::QWord
    );
    
    return std::make_shared<IRBinaryOperator>(
            BinaryOperator::PLUS,
            getVtablePointer,
            std::make_shared<IRConst>(this->offset)
        );


}