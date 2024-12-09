#include "ir_binary_operator.hh"

IRBinaryOperator::IRBinaryOperator(BinaryOperator o, std::shared_ptr<IRExpression> l,std::shared_ptr<IRExpression> r) : binop(o), left(std::move(l)), right(std::move(r))
{

}