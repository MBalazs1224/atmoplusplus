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

    // The return value needs to be moved into RAX and a ret instruction needs to be generated

    auto expressionAsIR = expression->TranslateExpressionToIr()->ToValueExpression();

    auto moveExpIntoRAX = std::make_shared<IRMove>(
        std::make_shared<IRTemp>(ReservedIrRegisters::RAX),
        expressionAsIR
    );

    auto ret = std::make_shared<IRReturn>();

    auto seq = std::make_shared<IRSequence>(
        moveExpIntoRAX,
        ret
    );



    return seq;
}