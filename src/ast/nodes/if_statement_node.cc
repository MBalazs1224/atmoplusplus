#include "if_statement_node.hh"

// Constructor: initializes the expression, body, else-if statements, and else block
IfStatementNode::IfStatementNode(std::shared_ptr<IExpressionable> expr_in, std::unique_ptr<BodyNode> body_in, std::vector<std::unique_ptr<ElseIfStatementNode>> else_ifs_in, std::unique_ptr<ElseStatementNode> else_in, yy::location loc)
    : Node(loc), expression(std::move(expr_in)), body(std::move(body_in)), else_ifs(std::move(else_ifs_in)), else_(std::move(else_in))
{
}

// Destructor: default implementation as no custom cleanup is required
IfStatementNode::~IfStatementNode() = default;

// Check function: validates the expression, body, else-if statements, and else block
bool IfStatementNode::Check()
{
    // Validate the if statement's expression
    if (!expression->Check())
    {
        return false;
    }

    // Get the type of the expression
    auto exp_type = expression->GetType();

    // Ensure the expression is of boolean type
    if (exp_type->NotCompatible(Helper::BooleanType))
    {
        Error::ShowError(Helper::FormatString("The expression of an if statement must be of type boolean! (received '%s')", exp_type->ToString().c_str()), expression->location);
        return false;
    }

    if(std::dynamic_pointer_cast<AssignmentExpression>(expression))
    {
       Error::ShowError("Cannot use assignment expression as a condition to an if statement!", expression->location);
       return false;
    }

    // Check if the body is empty, show a warning if so
    if (body->isEmpty())
    {
        Error::ShowWarning("Empty body of if statement!", location);
    }

    // Validate the body of the if statement
    if (!body->Check())
    {
        return false;
    }

    // Validate each else-if statement
    for (auto& else_if : else_ifs)
    {
        if (!else_if->Check())
        {
            return false;
        }
    }

    // Validate the else block if it exists
    if (else_)
    {
        if (!else_->Check())
        {
            return false;
        }
    }

    return true;
}

std::vector<std::shared_ptr<ReturnStatementNode>> IfStatementNode::GetReturnNodes()
{
    auto returnNodes = body->GetReturnNodes();

    for (auto& else_if : else_ifs)
    {
        auto elseIfReturns = else_if->GetReturnNodes();
        returnNodes.insert(returnNodes.end(), elseIfReturns.begin(), elseIfReturns.end());
    }

    if (else_)
    {
        auto elseReturns = else_->GetReturnNodes();
        returnNodes.insert(returnNodes.end(), elseReturns.begin(), elseReturns.end());
    }

    return returnNodes;
}

std::vector<std::shared_ptr<VariableSymbol>> IfStatementNode::GetVariables()
{
    // Get the variables from the body
    auto variables = body->GetVariables();

    // Get the variables from each else-if statement
    for (auto& else_if : else_ifs)
    {
        auto else_if_vars = else_if->GetVariables();
        variables.insert(variables.end(), else_if_vars.begin(), else_if_vars.end());
    }

    // Get the variables from the else block if it exists
    if (else_)
    {
        auto else_vars = else_->GetVariables();
        variables.insert(variables.end(), else_vars.begin(), else_vars.end());
    }

    return variables;
}


std::shared_ptr<IRStatement> IfStatementNode::TranslateToIR()
{
    std::shared_ptr<Label> endLabel = std::make_shared<Label>();

    std::shared_ptr<Label> ifTrueLabel = std::make_shared<Label>();

    std::shared_ptr<Label> firstFalseLabel = std::make_shared<Label>();

    std::vector<std::shared_ptr<IRStatement>> statements;

    std::shared_ptr<TranslateExpression> condExpr = expression->TranslateExpressionToIr();
    std::shared_ptr<IRStatement> condStmt = condExpr->ToConditionExpression(ifTrueLabel, firstFalseLabel);
    statements.push_back(condStmt);

    // True path for main if
    statements.push_back(std::make_shared<IRLabel>(ifTrueLabel));
    statements.push_back(body->TranslateToIR());
    statements.push_back(std::make_shared<IRJump>(std::make_shared<IRName>(endLabel)));

    // ------------- else-if chain -------------
    std::shared_ptr<Label> currentFalseLabel = firstFalseLabel;

    for (size_t i = 0; i < else_ifs.size(); ++i) {
        std::shared_ptr<Label> elseIfTrueLabel = std::make_shared<Label>();

        // Label to jump to if condition is false
        std::shared_ptr<Label> nextFalseLabel = (i + 1 < else_ifs.size() || else_) 
                                                ? std::make_shared<Label>() 
                                                : endLabel;

        // Condition
        auto elseIfCondExpr = else_ifs[i]->expression->TranslateExpressionToIr();
        auto elseIfCondStmt = elseIfCondExpr->ToConditionExpression(elseIfTrueLabel, nextFalseLabel);

        // False label:
        statements.push_back(std::make_shared<IRLabel>(currentFalseLabel));
        statements.push_back(elseIfCondStmt);

        // True path of this else-if
        statements.push_back(std::make_shared<IRLabel>(elseIfTrueLabel));
        statements.push_back(else_ifs[i]->body->TranslateToIR());
        statements.push_back(std::make_shared<IRJump>(std::make_shared<IRName>(endLabel)));

        currentFalseLabel = nextFalseLabel;
    }

    // ------------- else block -------------
    if (else_) {
        statements.push_back(std::make_shared<IRLabel>(currentFalseLabel));
        statements.push_back(else_->body->TranslateToIR());
        statements.push_back(std::make_shared<IRJump>(std::make_shared<IRName>(endLabel)));
    } else if (currentFalseLabel != endLabel) {
        statements.push_back(std::make_shared<IRLabel>(currentFalseLabel));
        // fall through to end
    }

    // Final label
    statements.push_back(std::make_shared<IRLabel>(endLabel));

    // std::shared_ptr<IRStatement> result = statements[0];
    // for (size_t i = 1; i < statements.size(); ++i) {
    //     result = std::make_shared<IRSequence>(result, statements[i]);
    // }

    return std::make_shared<IRSequence>(statements);
}
