#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

#include "statements/all_ir_statements.hh"
#include "expressions/all_ir_expressions.hh"

#include "../frame/temp.hh"

#include "../frame/global_frame.hh"

#include "blocks/ir_block.hh"
class IRNormalizer
{
public:
    // Main function to normalize the IR tree
    static std::shared_ptr<IRStatement> NormalizeTree(std::shared_ptr<IRStatement> root);

    /// @brief Will generate the IR blocks.
    /// @param normalized The normalized IR tree.
    /// @return Pointer to the first block.
    static std::shared_ptr<IRBlock> GenerateBlocks(std::shared_ptr<IRStatement> normalized);

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