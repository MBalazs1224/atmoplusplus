#include "ir_block.hh"

IRBlock::IRBlock(std::shared_ptr<IRStatementList> root)
{
    this->doneLabel = std::make_shared<Label>();
    MakeBlocks(root);
}

void IRBlock::AddStatement(std::shared_ptr<IRStatement> statement)
{
    lastStatement = lastStatement->tail = std::make_shared<IRStatementList>(statement,nullptr);
}

void IRBlock::DoStatements(std::shared_ptr<IRStatementList> statement)
{
    if (!statement)
    {

        DoStatements(
            std::make_shared<IRStatementList>(
                std::make_shared<IRJump>(
                    std::make_shared<IRName>(
                        doneLabel
                    )
                ),
                nullptr
            )
        );
    }
    // If the current statement is some kind of jump
    else if(std::dynamic_pointer_cast<IRJump>(statement->head) || std::dynamic_pointer_cast<IRCJump>(statement->head))
    {   
        AddStatement(statement->head);
        MakeBlocks(statement->tail);
    }
    // The statement is a label
    else if(auto printedLabel =std::dynamic_pointer_cast<IRLabel>(statement->head))
    {
        // Terminate the current block with a jump to the label.
        AddStatement(std::make_shared<IRJump>(
            std::make_shared<IRName>(printedLabel->label)
        ));
        // Now start a new block at this label.
        MakeBlocks(statement);
    }
    else
    {
        AddStatement(statement->head);
        DoStatements(statement->tail);
    }
    
}

void IRBlock::MakeBlocks(std::shared_ptr<IRStatementList> statement)
{
    if(!statement)
        return;

    // If the current statement is a label
    if(std::dynamic_pointer_cast<IRLabel>(statement->head))
    {
        lastStatement = std::make_shared<IRStatementList>(statement->head,nullptr);

        if(!lastBlock)
        {
            lastBlock = blocks = std::make_shared<IRStatementListList>(lastStatement,nullptr);
        }
        else
        {
            lastBlock = lastBlock->tail = std::make_shared<IRStatementListList>(lastStatement,nullptr);
        }

        DoStatements(statement->tail);
    }
    else
    {
        // Create a new block with a random name
        MakeBlocks(
            std::make_shared<IRStatementList>(
                std::make_shared<IRLabel>(
                    std::make_shared<Label>()
                ),
                statement
            )
        );
    }
}