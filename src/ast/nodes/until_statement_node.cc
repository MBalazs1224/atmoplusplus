#include "until_statement_node.hh"

UntilStatementNode::UntilStatementNode(std::shared_ptr<IExpressionable> expression_in, std::unique_ptr<BodyNode> body_in, yy::location loc)
    : Node(loc), body(std::move(body_in)), expression(std::move(expression_in))
{
}

UntilStatementNode::~UntilStatementNode() = default;

std::vector<std::shared_ptr<ReturnStatementNode>> UntilStatementNode::GetReturnNodes()
{
    return body->GetReturnNodes();
}

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

    if (std::dynamic_pointer_cast<AssignmentExpression>(expression))
    {
        Error::ShowError("Cannot use assignment expression as a condition to an until statement!", expression->location);
        return false;
    }

    if (body->isEmpty())
    {
        Error::ShowWarning("Empty body of until statement!", location);
    }

    return body->Check();
}

std::vector<std::shared_ptr<VariableSymbol>> UntilStatementNode::GetVariables()
{
    // Get the variables from the body
    return body->GetVariables();
}

std::shared_ptr<IRStatement> UntilStatementNode::TranslateToIR()
{
    // Label to indicate the start of the loop
    std::shared_ptr<Label> start_label = std::make_shared<Label>();

    // FIXME: Until-statement node might could be implemented with only 2 labels
    // Label indicating the start of the instructions
    std::shared_ptr<Label> instruction_label = std::make_shared<Label>();

    // Label pointing to the next instruction after the loop
    std::shared_ptr<Label> end_label = std::make_shared<Label>();

    // Vector containing all statements needed for this until-statement
    std::vector<std::shared_ptr<IRStatement>> statements;

    std::shared_ptr<TranslateExpression> conditionalExpression = expression->TranslateExpressionToIr();

    // FIXME:The condition might only need to just jump to the end if it's true and at the end of the body there should be a condition that jumps back to the start

    // The until-statement works the other way around compared to the generic while statement that's why it needs to jump to the end if the expression is true, otherwise jump back to the start and repeat the instructions


    auto conditionStatement = conditionalExpression->ToConditionExpression(end_label,instruction_label);


    // Generate the asm label for the start of the loop
    statements.push_back(std::make_shared<IRLabel>(start_label));

    // Execute the condition statements
    statements.push_back(conditionStatement);
    
    // Generate the asm label for the start of the instructions
    statements.push_back(std::make_shared<IRLabel>(instruction_label));

    // Insert all the statements of the body
    statements.push_back(body->TranslateToIR());

    // Jump back to the start of the loop, so we can check the expression again
    statements.push_back(std::make_shared<IRJump>(std::make_shared<IRName>(start_label)));

    // Generate the asm label for the end so the condition can jump out of the loop
    statements.push_back(std::make_shared<IRLabel>(end_label));

    // Convert the elements into a sequence tree
    return std::make_shared<IRSequence>(statements);


}