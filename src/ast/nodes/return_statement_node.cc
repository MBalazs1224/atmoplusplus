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

    // The return value needs to be moved into RAX then a leave and a ret instruction needs to be generated

    auto expressionAsIR = expression->TranslateExpressionToIr()->ToValueExpression();

    auto returnLocation = std::make_shared<IRTemp>(
        GlobalFrame::globalFrameType->ReturnLocation() // Get the return location from the global frame onject
    );

    auto moveExpIntoRAX = std::make_shared<IRMove>(
        returnLocation,
        expressionAsIR
    );

    auto ret = std::make_shared<IRReturn>();

    auto leave = std::make_shared<IRLeave>();

    auto leaveThenReturn = std::make_shared<IRSequence>(
        leave,
        ret
    );

    auto seq = std::make_shared<IRSequence>(
        moveExpIntoRAX,
        leaveThenReturn
    );



    return seq;
}