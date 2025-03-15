#include "ir_normalizer.hh"


std::shared_ptr<IRStatement> IRNormalizer::NormalizeTree(std::shared_ptr<IRStatement> root)
{
    // Step 1: Flatten nested SEQ structures
    std::vector<std::shared_ptr<IRStatement>> flattenedStatements = FlattenStatements(root);
    std::vector<std::shared_ptr<IRStatement>> finalResult;

    // Step 2: Normalize each statement in the flattened list
    for (auto &&statement : flattenedStatements)
    {
        auto normalized = NormalizeStatement(statement);
        finalResult.push_back(normalized);
    }
    
    // Step 3: Merge back into a single sequence
    return MergeStatements(finalResult);
}

// Flattens nested SEQ statements into a linear list
std::vector<std::shared_ptr<IRStatement>> IRNormalizer::FlattenStatements(std::shared_ptr<IRStatement> statement)
{
    std::vector<std::shared_ptr<IRStatement>> result;

    if (auto seq = std::dynamic_pointer_cast<IRSequence>(statement))
    {
        // Recursively flatten left and right branches
        auto leftBranch = FlattenStatements(seq->left);
        auto rightBranch = FlattenStatements(seq->right);

        result.insert(result.end(), leftBranch.begin(), leftBranch.end());
        result.insert(result.end(), rightBranch.begin(), rightBranch.end());
    }
    else
    {
        // If not a SEQ, add directly
        result.push_back(statement);
    }
    
    return result;
}

// Normalizes statements, specifically handling MOVE with ESEQ or CALL
std::shared_ptr<IRStatement> IRNormalizer::NormalizeStatement(std::shared_ptr<IRStatement> statement)
{
    if (auto move = std::dynamic_pointer_cast<IRMove>(statement))
    {
        std::vector<std::shared_ptr<IRStatement>> extractedStatements;

        // Case: Destination of MOVE is an ESEQ (must extract the statement first)
        if (auto eseq = std::dynamic_pointer_cast<IREseq>(move->destination))
        {
            auto normalizedStmt = NormalizeStatement(eseq->stmt);
            extractedStatements.push_back(normalizedStmt);
            move->destination = eseq->exp;  // Use the extracted expression
        }

        // Normalize both destination and source
        move->source = NormalizeExpression(move->source, extractedStatements);
        move->destination = NormalizeExpression(move->destination, extractedStatements);

        extractedStatements.push_back(move);
        return MergeStatements(extractedStatements);
    }
    else if (auto expStmt = std::dynamic_pointer_cast<IREvaluateExpression>(statement))
    {
        // Case: EXP statement containing CALL (ensure CALL is a statement)
        if (auto call = std::dynamic_pointer_cast<IRCall>(expStmt->exp))
        {
            std::vector<std::shared_ptr<IRStatement>> extractedStatements;
            NormalizeExpression(call, extractedStatements);
            return MergeStatements(extractedStatements);
        }
    }

    return statement;
}

// Normalizes expressions by extracting ESEQ statements and function calls
std::shared_ptr<IRExpression> IRNormalizer::NormalizeExpression(std::shared_ptr<IRExpression> expression,
    std::vector<std::shared_ptr<IRStatement>>& statements)
{
    if (auto eseq = std::dynamic_pointer_cast<IREseq>(expression))
    {
        // Flatten the statement part first
        auto normalizedStmt = NormalizeStatement(eseq->stmt);
        auto normalizedExp = NormalizeExpression(eseq->exp, statements);

        statements.push_back(normalizedStmt);
        return normalizedExp;
    }
    else if (auto functionCall = std::dynamic_pointer_cast<IRCall>(expression))
    {
        // Assign function call result to a temporary register
        auto temp = std::make_shared<Temp>();
        auto irTemp = std::make_shared<IRTemp>(temp);

        auto newMove = std::make_shared<IRMove>(irTemp, functionCall);
        statements.push_back(newMove);

        return irTemp;
    }
    
    return expression;
}

// Merges a vector of statements into a single SEQ statement
std::shared_ptr<IRStatement> IRNormalizer::MergeStatements(const std::vector<std::shared_ptr<IRStatement>>& stmts)
{
    if (stmts.empty()) return nullptr;
    std::shared_ptr<IRStatement> result = stmts[0];

    for (size_t i = 1; i < stmts.size(); ++i) {
        result = std::make_shared<IRSequence>(result, stmts[i]);
    }

    return result;
}
