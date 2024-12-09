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
    //TODO: Implement DoUntilStatementNode::TranslateToIR
    return nullptr;
}

