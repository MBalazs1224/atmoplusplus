#include "ir_evaluate_expression.hh"

IREvaluateExpression::IREvaluateExpression(std::shared_ptr<IRExpression> e) : exp(std::move(e))
{

}