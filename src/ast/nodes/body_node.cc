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

void BodyNode::AddStatements(std::vector<std::shared_ptr<IRStatement>> stmts)
{
    this->priorityStatements = std::move(stmts);
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
    // The body is empty if it doesn't have any normal or priority instructions
    return statements.empty() && priorityStatements.empty();
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

        else if (auto variable_container = std::dynamic_pointer_cast<BodyContainer>(statement))
        {
            auto vars = variable_container->GetVariables();
            variables.insert(variables.end(), vars.begin(), vars.end());
        }
    }
    return variables;
}

std::vector<std::shared_ptr<ReturnStatementNode>> BodyNode::GetReturnNodes()
{
    std::vector<std::shared_ptr<ReturnStatementNode>> variables;
    for (auto &statement : statements)
    {
        // If the statement is a variable definition, add tje variable defined by it to the list of variables
        if(auto returnNode = std::dynamic_pointer_cast<ReturnStatementNode>(statement))
        {
            variables.push_back(returnNode);
        }

        // If the statement is not a variable but can contain variables (if stmts, loops etc.), get the veriables from it recursively and add it to the vector

        else if (auto variable_container = std::dynamic_pointer_cast<BodyContainer>(statement))
        {
            auto vars = variable_container->GetReturnNodes();
            variables.insert(variables.end(), vars.begin(), vars.end());
        }
    }
    return variables;
}

std::vector<std::shared_ptr<VariableSymbol>> BodyNode::GetClassTypeVariables()
{
    std::vector<std::shared_ptr<VariableSymbol>> variables;

    for (auto &&statement : this->statements)
    {
        // The statement is a variable definition
        if(auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(statement))
        {
            auto var = varDef->GetVariable();

            auto varType = var->GetType();

            
            // If the variable is a class type push it to the vector
            if(std::dynamic_pointer_cast<ClassSymbol>(varType))
                variables.push_back(var);
        }
    }

    return variables;
    
}

std::shared_ptr<IRStatement> BodyNode::TranslateToIR()
{
    std::vector<std::shared_ptr<IRStatement>> ir_statements;

    // The first statements should be the priority statements
    ir_statements.insert(ir_statements.begin(), priorityStatements.begin(),priorityStatements.end());


    for (auto &&statement : statements)
    {
        auto ir = statement->TranslateToIR();
        if(ir) // There are some nodes that don't need translation to ir, like variable definitions
            ir_statements.push_back(ir);

    }

    // At the end of the body, we need to unregister every class type object

    auto classTypeVariables = GetClassTypeVariables();

    // Unregister the class type variables

    auto irNameForUnreg = std::make_shared<IRName>(
        std::make_shared<Label>("GCUnregisterRoot")
    );

    auto rdiLocation = std::make_shared<IRTemp>(ReservedIrRegisters::RDI);

    auto argumentLocationList = std::make_shared<IRExpressionList>(
        rdiLocation,
        nullptr
    );

    auto rbpTemp = std::make_shared<IRTemp>(ReservedIrRegisters::FramePointer);

    for (auto &&var : classTypeVariables)
    {
        auto argumentForUnreg = var->access->AsExpression(rbpTemp);

        auto argumentList = std::make_shared<IRExpressionList>(
            argumentForUnreg,
            nullptr
        );

        auto unregStatement = std::make_shared<IRCall>(
            irNameForUnreg,
            argumentList,
            argumentLocationList,
            false, // Return value not needed
            DataSize::QWord
        );

        auto evalUnreg = std::make_shared<IREvaluateExpression>(
            unregStatement
        );

        ir_statements.push_back(evalUnreg);
    }

    if(ir_statements.empty())
    {
        return nullptr; // FIXME: Returning nullptr for empty vector will result in a segfault!
    }

    if (ir_statements.size() == 1)
    {
        return ir_statements[0];
    }

    
    return std::make_shared<IRSequence>(ir_statements);
}