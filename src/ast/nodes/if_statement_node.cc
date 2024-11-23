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
