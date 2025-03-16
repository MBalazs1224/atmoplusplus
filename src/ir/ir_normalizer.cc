#include "ir_normalizer.hh"

std::shared_ptr<IRStatement> IRNormalizer::NormalizeTree(std::shared_ptr<IRStatement> root) {
    std::vector<std::shared_ptr<IRStatement>> flattened = FlattenStatements(root);
    std::vector<std::shared_ptr<IRStatement>> normalized;
    
    for (auto&& stmt : flattened) {
        auto processed = NormalizeStatement(stmt);
        if (processed) {
            auto subFlattened = FlattenStatements(processed);
            normalized.insert(normalized.end(), subFlattened.begin(), subFlattened.end());
        }
    }
    
    return MergeStatements(normalized);
}

std::vector<std::shared_ptr<IRStatement>> IRNormalizer::FlattenStatements(
    std::shared_ptr<IRStatement> statement) 
{
    std::vector<std::shared_ptr<IRStatement>> result;
    
    if (auto seq = std::dynamic_pointer_cast<IRSequence>(statement)) {
        auto left = FlattenStatements(seq->left);
        auto right = FlattenStatements(seq->right);
        result.insert(result.end(), left.begin(), left.end());
        result.insert(result.end(), right.begin(), right.end());
    }
    else if (statement) {
        result.push_back(statement);
    }
    
    return result;
}

std::shared_ptr<IRStatement> IRNormalizer::NormalizeStatement(
    std::shared_ptr<IRStatement> statement) 
{
    if (auto move = std::dynamic_pointer_cast<IRMove>(statement)) {
        std::vector<std::shared_ptr<IRStatement>> extracted;
        
        // Handle ESEQ in destination
        if (auto eseq = std::dynamic_pointer_cast<IREseq>(move->destination)) {
            auto normalizedStmt = NormalizeStatement(eseq->stmt);
            auto normalizedExp = NormalizeExpression(eseq->exp, extracted);
            extracted.push_back(normalizedStmt);
            move->destination = normalizedExp;
        }
        
        // Normalize source and destination
        move->source = NormalizeExpression(move->source, extracted);
        move->destination = NormalizeExpression(move->destination, extracted);
        
        extracted.push_back(move);
        return MergeStatements(extracted);
    }
    else if (auto exp = std::dynamic_pointer_cast<IREvaluateExpression>(statement)) {
        std::vector<std::shared_ptr<IRStatement>> extracted;
        auto normalizedExp = NormalizeExpression(exp->exp, extracted);
        
        // Changed: Replace EVAL(CALL) with MOVE + TEMP
        if (!extracted.empty()) {
            return MergeStatements(extracted);
        }
        return exp;
    }
    else if (auto cjump = std::dynamic_pointer_cast<IRCJump>(statement)) {
        std::vector<std::shared_ptr<IRStatement>> extracted;
    
        // Normalize left and right expressions
        if (cjump->left) {
            cjump->left = NormalizeExpression(cjump->left, extracted);
        }
        if (cjump->right) {
            cjump->right = NormalizeExpression(cjump->right, extracted);
        }
    
        // If no statements were extracted, return the CJUMP as-is
        if (extracted.empty()) {
            return cjump;
        }
    
        // Otherwise, merge extracted statements with the CJUMP
        extracted.push_back(cjump);
        return MergeStatements(extracted);
    }
    else if (auto label = std::dynamic_pointer_cast<IRLabel>(statement)) {
        // Labels don't need normalization
        return label;
    }
    else if (auto jump = std::dynamic_pointer_cast<IRJump>(statement)) {
        std::vector<std::shared_ptr<IRStatement>> extracted;
        jump->exp = NormalizeExpression(jump->exp, extracted);
        return MergeStatements(extracted);
    }
    
    return statement;
}

std::shared_ptr<IRExpression> IRNormalizer::NormalizeExpression(
    std::shared_ptr<IRExpression> exp,
    std::vector<std::shared_ptr<IRStatement>>& extracted) 
{
    if (auto eseq = std::dynamic_pointer_cast<IREseq>(exp)) {
        auto normalizedStmt = NormalizeStatement(eseq->stmt);
        auto normalizedExp = NormalizeExpression(eseq->exp, extracted);
        
        // Flatten the statement
        auto flattened = FlattenStatements(normalizedStmt);
        extracted.insert(extracted.end(), flattened.begin(), flattened.end());
        
        return normalizedExp;
    }
    else if (auto call = std::dynamic_pointer_cast<IRCall>(exp)) {

        // Handle function calls by storing result in temporary
        auto temp = std::make_shared<IRTemp>(std::make_shared<Temp>());
        
        // Normalize call arguments
        std::vector<std::shared_ptr<IRExpression>> normalizedArgs;

        auto currentArg = call->args;

        while (currentArg)
        {
            normalizedArgs.push_back(NormalizeExpression(currentArg->expression,extracted));
            currentArg = currentArg->next;
        }
        
        if(normalizedArgs.empty())
        {
            call->args = nullptr;
        }
        else
        {
            // Create the head of the linked list
            auto head = std::make_shared<IRExpressionList>();
            head->expression = normalizedArgs[0];

            // Use a pointer to traverse and build the list
            auto current = head;

            // Add the remaining arguments
            for (size_t i = 1; i < normalizedArgs.size(); i++)
            {
                current->next = std::make_shared<IRExpressionList>();
                current = current->next;
                current->expression = normalizedArgs[i];
            }

            // Ensure the last node's next pointer is nullptr
            current->next = nullptr;

            // Assign the head of the list to call->args
            call->args = head;
        }

        // Generate moving the params to the correct location
        auto currentParamLocation = call->argumentLocations;
        auto currentArgument = call->args;

        while(currentArgument)
        {
            auto move = std::make_shared<IRMove>(
                currentParamLocation->expression,
                currentArgument->expression
            );
            extracted.push_back(move);

            currentArgument = currentArgument->next;
            currentParamLocation = currentParamLocation->next;
        }

        // First evaluate the expression
        extracted.push_back(
            std::make_shared<IREvaluateExpression>(
                call
            )
        );

        // Only move the return value to a new location if it's needed
        if(call->returnValueNeeded)
        {

            auto returnLocation = GlobalFrame::globalFrameType->ReturnLocation();

            extracted.push_back(
                std::make_shared<IRMove>(
                    temp,
                    returnLocation
                )
            );
        }

        
        return temp;
    }
    else if (auto binop = std::dynamic_pointer_cast<IRBinaryOperator>(exp)) {
        binop->left = NormalizeExpression(binop->left, extracted);
        binop->right = NormalizeExpression(binop->right, extracted);
        return binop;
    }
    else if (auto mem = std::dynamic_pointer_cast<IRMem>(exp)) {
        mem->exp = NormalizeExpression(mem->exp, extracted);
        return mem;
    }
    else if (auto temp = std::dynamic_pointer_cast<IRTemp>(exp)) {
        return temp;  // No normalization needed
    }
    else if (auto constant = std::dynamic_pointer_cast<IRConst>(exp)) {
        return constant;  // No normalization needed
    }
    
    return exp;
}

std::shared_ptr<IRStatement> IRNormalizer::MergeStatements(
    const std::vector<std::shared_ptr<IRStatement>>& stmts) 
{
    if (stmts.empty()) return nullptr;
    
    std::shared_ptr<IRStatement> result = stmts[0];
    for (size_t i = 1; i < stmts.size(); ++i) {
        result = std::make_shared<IRSequence>(result, stmts[i]);
    }
    return result;
}



std::shared_ptr<IRBlock> IRNormalizer::GenerateBlocks(std::shared_ptr<IRStatement> normalized)
{
    
}