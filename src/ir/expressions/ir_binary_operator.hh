#pragma once
#include "ir_expression.hh"
#include "binary_operators.hh"
#include <memory>


// Binary operator applied to the expressions  (left is evaluated before right)
class IRBinaryOperator
{
        BinaryOperator binop;

        std::shared_ptr<IRExpression> left;
        std::shared_ptr<IRExpression> right;

        IRBinaryOperator(BinaryOperator, std::shared_ptr<IRExpression>,std::shared_ptr<IRExpression>);
};