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
