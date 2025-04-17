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
    // The label for true path
    std::shared_ptr<Label> if_true = std::make_shared<Label>();

    // The label for false path (else-if or else)
    std::shared_ptr<Label> if_false = std::make_shared<Label>();

    // Label for joining all paths together
    std::shared_ptr<Label> if_end = std::make_shared<Label>();

    // Generate IR for the condition expression
    std::shared_ptr<TranslateExpression> conditionExpression = expression->TranslateExpressionToIr();

    std::shared_ptr<IRStatement> conditionStatement = conditionExpression->ToConditionExpression(if_true, if_false);

    // Will contain all the IR statements for this if-statement
    std::vector<std::shared_ptr<IRStatement>> statements;

    // Push the condition statement to the statements
    statements.push_back(conditionStatement);

    // -------- Translate the main `if` block

    // Print label for the true path
    statements.push_back(std::make_shared<IRLabel>(if_true));

    // Translate the body of the if path
    statements.push_back(body->TranslateToIR());

    // Jump to the end label
    statements.push_back(std::make_shared<IRJump>(std::make_shared<IRName>(if_end)));

    // Print label for false path
    //statements.push_back(std::make_shared<IRLabel>(if_false));

    // -------- Translate the `else if` blocks

    for (size_t i = 0; i < else_ifs.size(); i++)
    {
        // Create a new label for the true path of this else_if
        if_true = std::make_shared<Label>();

        // If there is still a next else_if then we need to jump to that otherwise jump to else path
        std::shared_ptr<Label> next_false = i + 1 < else_ifs.size() ? std::make_shared<Label>() : if_false;

        // Translate the else-if condition
        std::shared_ptr<TranslateExpression> elseIfConditionalExpression = else_ifs[i]->expression->TranslateExpressionToIr();
        std::shared_ptr<IRStatement> elseIfConditionalStatement = elseIfConditionalExpression->ToConditionExpression(if_true, next_false);

        // Add condition check
        statements.push_back(elseIfConditionalStatement);

        // Label for true path and body
        statements.push_back(std::make_shared<IRLabel>(if_true));
        statements.push_back(else_ifs[i]->body->TranslateToIR());

        // Jump to the final join label
        statements.push_back(std::make_shared<IRJump>(std::make_shared<IRName>(if_end)));

        // Update `if_false` for the next else_if
        if_false = next_false;
    }

    // -------- Translate else

    statements.push_back(std::make_shared<IRLabel>(if_false)); 
    if (else_)
    {
        statements.push_back(else_->body->TranslateToIR());
        // Jump out of the if statement to the next statement
        statements.push_back(std::make_shared<IRJump>(
            std::make_shared<IRName>(
                if_end
            )
        ));
    }

    // Print joining label
    statements.push_back(std::make_shared<IRLabel>(if_end));

    return std::make_shared<IRSequence>(statements);
}
