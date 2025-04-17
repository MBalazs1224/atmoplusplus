#include "else_statement_node.hh"

// Constructor: initializes the body for the else statement
ElseStatementNode::ElseStatementNode(std::unique_ptr<BodyNode> body_in, yy::location loc)
    : Node(loc), body(std::move(body_in))
{
}

// Destructor: uses default implementation as no custom cleanup is needed
ElseStatementNode::~ElseStatementNode() = default;

// Check function: validates the body in the else statement
bool ElseStatementNode::Check()
{
    return body->Check();
}

std::vector<std::shared_ptr<ReturnStatementNode>> ElseStatementNode::GetReturnNodes()
{
    return body->GetReturnNodes();
}

std::vector<std::shared_ptr<VariableSymbol>> ElseStatementNode::GetVariables()
{
    // Get the variables from the body
    return body->GetVariables();
}

std::shared_ptr<IRStatement> ElseStatementNode::TranslateToIR()
{
    throw std::logic_error("Else-statements should be translated in the IfStatementNode::TranslateToIR function!");
}
