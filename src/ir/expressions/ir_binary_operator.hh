#pragma once
#include "ir_expression.hh"
#include "binary_operators.hh"
#include <memory>


// Binary operator applied to the expressions  (left is evaluated before right)
class IRBinaryOperator
{
        BinaryOperator binop;

        std::unique_ptr<IRExpression> left;
        std::unique_ptr<IRExpression> right;

        IRBinaryOperator(BinaryOperator, std::unique_ptr<IRExpression>,std::unique_ptr<IRExpression>);
};