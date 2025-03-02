#pragma once
#include <memory>
class BoolList
{
    public:
        bool head;
        // BoolList will be used in a Frame object when it's allocating variable space for a frame, so this will contain the current variable's size
        int sizeOfVariable;
        std::shared_ptr<BoolList> tail;
        BoolList(bool head_in, std::shared_ptr<BoolList> tail_in);
};