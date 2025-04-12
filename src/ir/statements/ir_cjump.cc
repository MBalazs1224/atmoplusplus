#include "ir_cjump.hh"

IRCJump::IRCJump(RelationalOperator op, std::shared_ptr<IRExpression> l, std::shared_ptr<IRExpression>r, std::shared_ptr<Label> t, std::shared_ptr<Label> f) : relop(op), left(std::move(l)), right(std::move(r)), iftrue(t), iffalse(f)
{

}

std::shared_ptr<IRStatement> IRCJump::Build(std::shared_ptr<IRExpressionList> kids)
{
    return std::make_shared<IRCJump>(
        relop,
        kids->expression,
        kids->next->expression,
        iftrue,
        iffalse
    );
}

std::shared_ptr<IRExpressionList> IRCJump::Kids()
{
    return std::make_shared<IRExpressionList>(
        left,
        std::make_shared<IRExpressionList>(
            right,
            nullptr
        )
    );
}

RelationalOperator IRCJump::OppositeOperator(RelationalOperator op)
{
    switch (op)
    {
    case RelationalOperator::Matches:
       return RelationalOperator::NotMatches;

    case RelationalOperator::NotMatches:
       return RelationalOperator::Matches;

    case RelationalOperator::LessThan:
        return RelationalOperator::GreaterThanOrEqual;

    case RelationalOperator::GreaterThanOrEqual:
        return RelationalOperator::LessThan;

    case RelationalOperator::GreaterThan:
        return RelationalOperator::LessThanOrEqual;
        
    case RelationalOperator::LessThanOrEqual:
        return RelationalOperator::GreaterThan;

    default:
        throw std::logic_error("Bad relop in CJUMP::OppositeOperator!");
    }
}

std::string IRCJump::ToDotFormat(int &nodeCounter)
{
    int myId = nodeCounter++;
    std::string dot = "node" + std::to_string(myId) + " [label=\"CJUMP: " + OperatorToString() + "\"];\n";

    // Connect left operand
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
    dot += left->ToDotFormat(nodeCounter);

    // Connect right operand
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(nodeCounter) + ";\n";
    dot += right->ToDotFormat(nodeCounter);

    // Connect true label
    int trueId = nodeCounter++;
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(trueId) + " [label=\"TRUE\"];\n";
    dot += "node" + std::to_string(trueId) + " [label=\"LABEL: " + iftrue->ToString() + "\"];\n";

    // Connect false label
    int falseId = nodeCounter++;
    dot += "node" + std::to_string(myId) + " -> node" + std::to_string(falseId) + " [label=\"FALSE\"];\n";
    dot += "node" + std::to_string(falseId) + " [label=\"LABEL: " + iffalse->ToString() + "\"];\n";

    return dot;
}

std::string IRCJump::OperatorToString()
{
    switch (relop)
    {
    case RelationalOperator::GreaterThan:
        return "GreaterThan";

    case RelationalOperator::GreaterThanOrEqual:
        return "GreaterThanOrEqual";

    case RelationalOperator::LessThan:
        return "LessThan";

    case RelationalOperator::LessThanOrEqual:
        return "LessThanOrEqual";

    case RelationalOperator::Matches:
        return "Matches";

    case RelationalOperator::NotMatches:
        return "NotMatches";

    case RelationalOperator::Not:
        return "Not";
    
    default:
        throw std::logic_error("Unknown Relop!");
    }
}