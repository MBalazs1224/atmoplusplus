#include "inframe.hh"

InFrame::InFrame(int offset_in) : offset(offset_in)
{
}

std::string InFrame::ToString()
{
    return std::to_string(offset) + "(fp)";
}

std::shared_ptr<IRExpression> InFrame::AsExpression(std::shared_ptr<IRExpression> framePointer)
{
    // Returns the memory address at StackPointer + offset

    return std::make_shared<IRMem>(
        std::make_shared<IRBinaryOperator>(
            BinaryOperator::MINUS,
            framePointer,
            std::make_shared<IRConst>(offset)
        ));
}

std::shared_ptr<IRExpression> InFrame::AsExpressionNoDereference(std::shared_ptr<IRExpression> framePointer)
{
    // Don't dereference the location

    return std::make_shared<IRBinaryOperator>(
        BinaryOperator::PLUS,
        framePointer,
        std::make_shared<IRConst>(offset)
    );
}

