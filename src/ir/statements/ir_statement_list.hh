#pragma once
#include <memory>
#include "ir_statement.hh"
#include "../ir_node.hh"


class IRStatementList : public IRNode
{
    public:
        std::shared_ptr<IRStatement> head;
        std::shared_ptr<IRStatementList> tail;

        IRStatementList(std::shared_ptr<IRStatement>,std::shared_ptr<IRStatementList>);

        IRStatementList();

        std::string ToDotFormat(int&) override;
};