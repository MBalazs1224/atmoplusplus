#pragma once
#include <memory>
class BoolList
{
    public:
        bool head;
        std::shared_ptr<BoolList> tail;
        BoolList(bool head_in, std::shared_ptr<BoolList> tail_in);
};