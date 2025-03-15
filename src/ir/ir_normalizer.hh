#pragma once
#include <vector>
#include <memory>

#include "statements/ir_statement.hh"
#include "statements/ir_move.hh"
#include "statements/ir_sequence.hh"
#include "statements/ir_evaluate_expression.hh"
#include "expressions/ir_expression.hh"
#include "expressions/ir_eseq.hh"
#include "expressions/ir_call.hh"
#include "expressions/ir_temp.hh"

#include "../frame/temp.hh"
class IRNormalizer
{
public:
    // Main function to normalize the IR tree
    static std::shared_ptr<IRStatement> NormalizeTree(std::shared_ptr<IRStatement> root);

private:
    // Flattens nested SEQ structures into a list of statements
    static std::vector<std::shared_ptr<IRStatement>> FlattenStatements(std::shared_ptr<IRStatement> statement);

    // Normalizes an individual statement
    static std::shared_ptr<IRStatement> NormalizeStatement(std::shared_ptr<IRStatement> statement);

    // Normalizes an expression, extracting necessary statements (ESEQ handling, function calls)
    static std::shared_ptr<IRExpression> NormalizeExpression(
        std::shared_ptr<IRExpression> expression,
        std::vector<std::shared_ptr<IRStatement>> &statements);

    // Merges a vector of statements into a single SEQ
    static std::shared_ptr<IRStatement> MergeStatements(const std::vector<std::shared_ptr<IRStatement>> &stmts);
};