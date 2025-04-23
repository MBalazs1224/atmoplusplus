#include "inframe.hh"

InFrame::InFrame(int offset_in, int sizeIn) : offset(offset_in), size(sizeIn)
{
}

std::string InFrame::ToString()
{
    return std::to_string(offset) + "(fp)";
}

std::shared_ptr<IRExpression> InFrame::AsExpression(std::shared_ptr<IRExpression> framePointer)
{
    // Returns the memory address at FramePointer - offset

    return std::make_shared<IRMem>(
        std::make_shared<IRBinaryOperator>(
            BinaryOperator::MINUS,
            framePointer,
            std::make_shared<IRConst>(offset)
        ),
        this->size
    
    );
}

std::shared_ptr<IRExpression> InFrame::AsExpressionNoDereference(std::shared_ptr<IRExpression> framePointer)
{
    // Don't dereference the location

    return std::make_shared<IRBinaryOperator>(
        BinaryOperator::MINUS,
        framePointer,
        std::make_shared<IRConst>(offset)
    );
}

