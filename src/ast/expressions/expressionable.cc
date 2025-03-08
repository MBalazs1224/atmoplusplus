#include "expressionable.hh"

IExpressionable::~IExpressionable() = default;



std::shared_ptr<TranslateExpression> IExpressionable::TranslateExpressionToIrNoDereference()
{
    // For most expressions the normal TranslateExpressionToIr will do, so they can just inherit this wrapper

    return TranslateExpressionToIr();
}