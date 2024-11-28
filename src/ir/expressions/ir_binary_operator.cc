#include "ir_binary_operator.hh"

IRBinaryOperator::IRBinaryOperator(BinaryOperator o, std::unique_ptr<IRExpression> l,std::unique_ptr<IRExpression> r) : binop(o), left(std::move(l)), right(std::move(r))
{

}