#include "ir_trace.hh"

std::shared_ptr<IRStatementList> IRTraceSchedule::GetLast(std::shared_ptr<IRStatementList> block)
{
    auto list = block;
    while (list->tail->tail)
    {
        list = list->tail;
    }

    return list;
    
}

void IRTraceSchedule::Trace(std::shared_ptr<IRStatementList> list)
{
    while (true)
    {
        auto irLabel = std::dynamic_pointer_cast<IRLabel>(list->head);

        table.erase(irLabel->label);

        auto last = GetLast(list);
        auto statement = last->tail->head;

        if(auto jump = std::dynamic_pointer_cast<IRJump>(statement))
        {
            auto target = table[jump->targets->head];

            if(!jump->targets->tail && target)
            {
                last->tail = target;
                list = target;
            }
            else
            {
                last->tail->tail = GetNext();
                return;
            }
            
        }
        else if (auto cJump = std::dynamic_pointer_cast<IRCJump>(statement))
        {
            auto truePath = table[cJump->iftrue]; 
            auto falsePath = table[cJump->iffalse];
            
            if(falsePath)
            {
                last->tail->tail = falsePath;
                list = falsePath;
            }
            else if(truePath)
            {
                // Switch  operator and labels
                last->tail->head = std::make_shared<IRCJump>(
                    cJump->OppositeOperator(cJump->relop),
                    cJump->left,
                    cJump->right,
                    cJump->iffalse,
                    cJump->iftrue
                );

                last->tail->tail = truePath;
                list = truePath;
            }
            else
            {
                auto newLabel = std::make_shared<Label>();

                last->tail->head = std::make_shared<IRCJump>(
                    cJump->relop,
                    cJump->left,
                    cJump->right,
                    cJump->iftrue,
                    newLabel
                );

                last->tail->tail = std::make_shared<IRStatementList>(
                    std::make_shared<IRLabel>(
                        newLabel
                    ),
                    std::make_shared<IRStatementList>(
                        std::make_shared<IRJump>(
                            std::make_shared<IRName>(
                            cJump->iffalse
                                
                            )
                        ),
                        GetNext()
                    )
                );

                return;
            }
            
            
        }

        else
        {
            throw std::logic_error("Bad basic block in TraceSchedule");
        }
        
        
    }
    
}

std::shared_ptr<IRStatementList> IRTraceSchedule::GetNext()
{
    if (!block->blocks)
    {
        return std::make_shared<IRStatementList>(
            std::make_shared<IRLabel>(
                block->doneLabel
            ),
            nullptr
        );
    }

    auto statement = block->blocks->head;
    auto irLabel = std::dynamic_pointer_cast<IRLabel>(statement->head);

    auto tableContains = table.find(irLabel->label) != table.end();
    if(tableContains)
    {
        Trace(statement);
        return statement;
    }
    else
    {
        this->block->blocks = this->block->blocks->tail;
        return GetNext();
    }
    
}

IRTraceSchedule::IRTraceSchedule(std::shared_ptr<IRBlock> block)
{
    this->block = block;
    for (std::shared_ptr<IRStatementListList> statement = block->blocks; statement != nullptr; statement = statement->tail)
    {
        auto irLabel = std::dynamic_pointer_cast<IRLabel>( statement->head->head);

        auto label = irLabel->label;

        table[label] = statement->head; 
    }

    statements = GetNext();

    
}