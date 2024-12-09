#include "body_node.hh"
#include "function_definition_node.hh"

BodyNode::BodyNode(BodyNode &&other)
{
    this->statements = std::move(other.statements);
}

BodyNode::BodyNode(std::vector<std::shared_ptr<Node>> statements)
{
    this->statements = std::move(statements);
}

bool BodyNode::Check()
{
    // TODO: Might need to calculate the size of teh body here
    for (auto &statement : statements)
    {
        if(!StatementIsValid(statement))
        {
            Error::ShowError("Invalid statement!",statement->location);
            continue;
        }
        // Functions can only be created on the root node, meaning it cannot be inside a body (classes receive the statements inside them through a vector not a body )

        statement->Check();
    }
    return true;
}

bool BodyNode::StatementIsValid(const std::shared_ptr<Node> statement)
{
    if (auto function_definition = std::dynamic_pointer_cast<FunctionDefinitionNode>(statement))
    {
        Error::ShowError("Functions can only be created on the root level!", statement->location);
        return false;
    }

    if (auto class_definition = std::dynamic_pointer_cast<ClassDefinitionNode>(statement))
    {
        Error::ShowError("Functions can only be created on the root level!", statement->location);
        return true;
    }

    if(auto expression = std::dynamic_pointer_cast<IExpressionable>(statement))
    {
        // Only assignment expressions and function calls can be used as a statement 

        return std::dynamic_pointer_cast<AssignmentExpression>(expression) != nullptr || std::dynamic_pointer_cast<FunctionCall>(expression) != nullptr;
    }

    // The only node not legal as a statement is constructor definition

    return std::dynamic_pointer_cast<ConstructorDefinitionNode>(statement) == nullptr;
}

bool BodyNode::isEmpty()
{
    return statements.empty();
}

std::vector<std::shared_ptr<Node>> BodyNode::GetStatements()
{
    return statements;
}

std::vector<std::shared_ptr<VariableSymbol>> BodyNode::GetVariables()
{
    std::vector<std::shared_ptr<VariableSymbol>> variables;
    for (auto &statement : statements)
    {
        // If the statement is a variable definition, add tje variable defined by it to the list of variables
        if(auto variable_definition = std::dynamic_pointer_cast<VariableDefinitionNode>(statement))
        {
            variables.push_back(variable_definition->GetVariable());
        }

        // If the statement is not a variable but can contain variables (if stmts, loops etc.), get the veriables from it recursively and add it to the vector

        else if (auto variable_container = std::dynamic_pointer_cast<VariableContainer>(statement))
        {
            auto vars = variable_container->GetVariables();
            variables.insert(variables.end(), vars.begin(), vars.end());
        }
    }
    return variables;
}

std::shared_ptr<IRStatement> BodyNode::TranslateToIR()
{
    // std::vector<std::shared_ptr<IRStatement>> irStatements;
    // for (auto &statement : statements)
    // {
    //     auto irStatement = statement->TranslateToIR();
    //     if(irStatement != nullptr)
    //     {
    //         irStatements.push_back(irStatement);
    //     }
    // }
    // return std::make_shared<IRSeq>(irStatements);

    //TODO: Implement BodyNode TranslateToIR
    return nullptr;
}