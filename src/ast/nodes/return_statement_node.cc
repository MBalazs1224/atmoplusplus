#include "return_statement_node.hh"

ReturnStatementNode::ReturnStatementNode(std::shared_ptr<IExpressionable> exp_in, yy::location loc)
    : Node(loc), expression(std::move(exp_in))
{
}

ReturnStatementNode::~ReturnStatementNode() = default;

bool ReturnStatementNode::Check()
{
    // TODO: Implement return node checking logic as needed
    return true;
}


std::shared_ptr<IRStatement> ReturnStatementNode::TranslateToIR()
{
    //TODO: Implement ReturnStatementNode TranslateToIR

    return nullptr;
}