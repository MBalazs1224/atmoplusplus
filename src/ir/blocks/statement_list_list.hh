#pragma once

#include "../statements/ir_statement_list.hh"

class IRStatementListList
{
    public:
        std::shared_ptr<IRStatementList> head;
        std::shared_ptr<IRStatementListList> tail;

        IRStatementListList(std::shared_ptr<IRStatementList> headIn, std::shared_ptr<IRStatementListList>tailIn) : head(headIn), tail(tailIn) {}
};