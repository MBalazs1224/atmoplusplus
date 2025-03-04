#include "access.hh"

std::shared_ptr<IRExpression> Access::AsExpressionNoDereference(std::shared_ptr<IRExpression> startOffset)
{
    // Most access object doesn't use dereference so the can inehrit this function and automatically use the normal function.
    return AsExpression(startOffset);
}