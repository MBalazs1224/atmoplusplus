#include "ir_evaluate_expression.hh"

IREvaluateExpression::IREvaluateExpression(std::unique_ptr<IRExpression> e) : exp(std::move(e))
{

}