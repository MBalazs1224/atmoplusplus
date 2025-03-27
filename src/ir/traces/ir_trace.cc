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
            auto target = GetElementFromMap(jump->targets->head);

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
            auto truePath = GetElementFromMap(cJump->iftrue);
            auto falsePath = GetElementFromMap(cJump->iffalse);
            
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

    auto element = GetElementFromMap(irLabel->label);
    if(element)
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

std::shared_ptr<IRStatementList> IRTraceSchedule::GetElementFromMap(std::shared_ptr<Label> key)
{
    // Return nullptr if the key doesn't exists
    auto contains = table.find(key) != table.end();
    if(!contains)
        return nullptr;

    return table.at(key);
}

IRTraceSchedule::IRTraceSchedule(std::shared_ptr<IRBlock> block)
{
    this->block = block;
    for (std::shared_ptr<IRStatementListList> statement = block->blocks; statement != nullptr; statement = statement->tail)
    {
        auto irLabel = std::dynamic_pointer_cast<IRLabel>( statement->head->head);

        auto label = irLabel->label;

        table.insert({label,statement->head});
    }

    statements = GetNext();


}