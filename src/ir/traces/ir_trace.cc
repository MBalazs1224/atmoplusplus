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

void IRTraceSchedule::Trace(std::shared_ptr<IRStatementList> list) {
    while (true) {
        // Remove label from scheduling table
        auto curLabel = std::dynamic_pointer_cast<IRLabel>(list->head);
        table.erase(curLabel->label);

        auto lastNode = GetLast(list);
        auto stmt = lastNode->tail->head;

        if (auto uj = std::dynamic_pointer_cast<IRJump>(stmt)) {
            // Handle unconditional jumps
            if (uj->targets->tail) {
                // Multiple targets - end trace
                lastNode->tail->tail = GetNext();
                return;
            }

            auto target = GetElementFromMap(uj->targets->head);
            if (target) {
                // Single unscheduled target - append directly
                lastNode->tail = target;
                list = target;
            } else {
                // Target already scheduled - end trace
                lastNode->tail->tail = GetNext();
                return;
            }
        }
        else if (auto cj = std::dynamic_pointer_cast<IRCJump>(stmt)) {
            auto falseBlk = GetElementFromMap(cj->iffalse);
            auto trueBlk  = GetElementFromMap(cj->iftrue);

            if (trueBlk && !falseBlk) {
                // True path available; make true fall-through
                lastNode->tail->head = std::make_shared<IRCJump>(
                    cj->OppositeOperator(cj->relop), // flip condition
                    cj->left,
                    cj->right,
                    cj->iffalse,  // now taken if inverted condition is true
                    cj->iftrue    // fall-through
                );
                lastNode->tail->tail = trueBlk;
                list = trueBlk;
            }
            else if (falseBlk) {
                // False path is fall-through
                lastNode->tail->head = cj;
                lastNode->tail->tail = falseBlk;
                list = falseBlk;
            }
            else {
                // Both paths already scheduled or unavailable
                lastNode->tail->tail = GetNext();
                return;
            }
        }
        else {
            throw std::logic_error("TraceSchedule saw non-jump at block end!");
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