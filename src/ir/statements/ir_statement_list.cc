#include "ir_statement_list.hh"

IRStatementList::IRStatementList(std::shared_ptr<IRStatement> head_in, std::shared_ptr<IRStatementList> tail_in) : head(head_in), tail(tail_in)
{
}