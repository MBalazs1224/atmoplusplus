#include "do_until_statement_node.hh"

// Constructor: initializes the expression and body nodes
DoUntilStatementNode::DoUntilStatementNode(std::shared_ptr<IExpressionable> expression_in, std::unique_ptr<BodyNode> body_in, yy::location loc)
    : Node(loc), body(std::move(body_in)), expression(std::move(expression_in))
{
}

// Destructor: uses default implementation as no custom cleanup is needed
DoUntilStatementNode::~DoUntilStatementNode() = default;

// Check function: verifies the validity of the expression and body in the do-until statement
bool DoUntilStatementNode::Check()
{
    // Check if the expression is valid
    if (!expression->Check())
    {
        return false;
    }

    // Get the type of the expression
    auto exp_type = expression->GetType();

    // Ensure the expression is of boolean type
    if (exp_type->NotCompatible(Helper::BooleanType))
    {
        Error::ShowError(Helper::FormatString("The expression of a do-until statement must be of type boolean! (received '%s')", exp_type->ToString().c_str()), expression->location);
        return false;
    }

    if (std::dynamic_pointer_cast<AssignmentExpression>(expression))
    {
        Error::ShowError("Cannot use assignment expression as a condition to a do-until statement!", expression->location);
        return false;
    }

    // Check if the body is empty and show a warning if so
    if (body->isEmpty())
    {
        Error::ShowWarning("Empty body of do-until statement!", location);
    }

    // Check the validity of the body
    return body->Check();
}

std::vector<std::shared_ptr<VariableSymbol>> DoUntilStatementNode::GetVariables()
{
    // Get the variables from the body
    return body->GetVariables();
}

std::shared_ptr<IRStatement> DoUntilStatementNode::TranslateToIR()
{

    // The do-until statement will execute the body at least once (just like a do-while in a regular language)

    // Label for the start of the do-until statement
    std::shared_ptr<Label> do_until_start = std::make_shared<Label>();

    // Label for the end of the instructions
    std::shared_ptr<Label> do_until_end = std::make_shared<Label>();

    std::vector<std::shared_ptr<IRStatement>> statements;

    // Generate IR for the body of the do-until statement
    std::shared_ptr<IRStatement> body_statements = body->TranslateToIR();


    // Generate IR for the condition expression

    auto condition_expression = expression->TranslateExpressionToIr();

    // Jump to the start of the do-until statement if the given expression is false, otherwise jump out the loop
    auto condition_statement = condition_expression->ToConditionExpression(do_until_end,do_until_start);


    // Generate the asm label for the start of the do-until statement
    statements.push_back(std::make_shared<IRLabel>(do_until_start));

    // Execute the IR for the body
    statements.push_back(body_statements);

    // Execute the condition
    statements.push_back(condition_statement);

    // Generate label for the end of the do-until statement
    statements.push_back(std::make_shared<IRLabel>(do_until_end));


    return std::make_shared<IRSequence>(statements);

}

