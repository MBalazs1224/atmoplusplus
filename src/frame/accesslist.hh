#pragma once
#include "access.hh"
#include <memory>
class AccessList
{
    public:
        std::shared_ptr<Access> head;
        std::shared_ptr<AccessList> tail;
        AccessList(std::shared_ptr<Access> head_in, std::shared_ptr<AccessList> tail_in);
};