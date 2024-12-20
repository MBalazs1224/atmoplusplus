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
    // The label that will point to the statements that needs to be executed if the statement is true
    std::shared_ptr<Label> if_true = std::make_shared<Label>();

    // The label for the else-if or else branch 
    std::shared_ptr<Label> if_false = std::make_shared<Label>();

    // Label for joining together all branches
    std::shared_ptr<Label> if_end = std::make_shared<Label>();


    // Generate IR for the condition expression
    std::shared_ptr<TranslateExpression> conditionExpression = expression->TranslateExpressionToIr();

    // Will convert the condition expression into a condition jump
    std::shared_ptr<IRStatement> conditionStatement = conditionExpression->ToConditionExpression(if_true,if_false);

    // Will contain all the IR statements for this if-statement
    std::vector<std::shared_ptr<IRStatement>> statements;

    // Push the condition statement to the statements
    statements.push_back(conditionStatement);




    // -------- Translate the main if

    // Create asm label for the "true" path
    statements.push_back(std::make_shared<IRLabel>(if_true));

    // Statements inside the body of the "true" path
    statements.push_back(body->TranslateToIR());

    // Jump to the joining end label at the end of the body
    statements.push_back(std::make_shared<IRJump>(if_end));

    // Generate the false label so it can jump to the else-ifs if the main if is false

    statements.push_back(std::make_shared<IRLabel>(if_false));

    // -------- Translate the else-ifs

    for (auto &&elseIf : else_ifs)
    {
        


        // Create a new label for the "true" and "false" parts
        if_true = std::make_shared<Label>();
        if_false = std::make_shared<Label>();

        // Translate the else-if's condition
        std::shared_ptr<TranslateExpression> elseIfConditionalExpression = elseIf->expression->TranslateExpressionToIr();

        std::shared_ptr<IRStatement> elseIfConditionalStatement = elseIfConditionalExpression->ToConditionExpression(if_true,if_false);

        // Asm label and body for this else-if branch

        statements.push_back(std::make_shared<IRLabel>(if_true));
        statements.push_back(elseIf->body->TranslateToIR());

        // Jump to the joining end label
        statements.push_back(std::make_shared<IRJump>(if_end));

        // Jump to the next else-if
        statements.push_back(std::make_shared<IRJump>(if_false));
    }

    // -------- Translate the else

    // Create label for the final "false" aprt
    statements.push_back(std::make_shared<IRLabel>(if_false));

    // Translate the else body if exists

    if(else_)
    {
        statements.push_back(else_->body->TranslateToIR());
    }

    
    // Translate the final joining label

    statements.push_back(std::make_shared<IRLabel>(if_end));

    return std::make_shared<IRSequence>(statements);


}