#include "else_if_statement_node.hh"

ElseIfStatementNode::ElseIfStatementNode(std::shared_ptr<IExpressionable> expression_in, std::unique_ptr<BodyNode> body_in, yy::location loc)
    : Node(loc), expression(std::move(expression_in)), body(std::move(body_in))
{
}

ElseIfStatementNode::~ElseIfStatementNode() = default;

bool ElseIfStatementNode::Check()
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
        Error::ShowError(Helper::FormatString("The expression of an else-if statement must be of type boolean! (received '%s')", exp_type->ToString().c_str()), expression->location);
        return false;
    }

    // Check if the body is empty and issue a warning if so
    if (body->isEmpty())
    {
        Error::ShowWarning("Empty body of else-if statement!", location);
    }

    // Check the validity of the body
    return body->Check();
}

std::vector<std::shared_ptr<VariableSymbol>> ElseIfStatementNode::GetVariables()
{
    // Get the variables from the body
    return body->GetVariables();
}