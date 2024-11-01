#include "until_statement_node.hh"

UntilStatementNode::UntilStatementNode(std::shared_ptr<IExpressionable> expression_in, std::unique_ptr<BodyNode> body_in, yy::location loc)
    : Node(loc), body(std::move(body_in)), expression(std::move(expression_in))
{
}

UntilStatementNode::~UntilStatementNode() = default;

bool UntilStatementNode::Check()
{
    if (!expression->Check())
    {
        return false;
    }

    auto exp_type = expression->GetType();

    if (exp_type->NotCompatible(Helper::BooleanType))
    {
        Error::ShowError(Helper::FormatString("The expression of an until statement must be of type boolean! (received '%s')", exp_type->ToString().c_str()), expression->location);
        return false;
    }

    if (body->isEmpty())
    {
        Error::ShowWarning("Empty body of until statement!", location);
    }

    return body->Check();
}
