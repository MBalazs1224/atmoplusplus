#pragma once
#include <memory>
#include "ir_statement.hh"


class IRStatementList
{
    public:
        std::shared_ptr<IRStatement> head;
        std::shared_ptr<IRStatementList> tail;

        IRStatementList(std::shared_ptr<IRStatement>,std::shared_ptr<IRStatementList>);

};