#pragma once
#include <memory>
#include "label.hh"
class LabelList
{
    public:
        std::shared_ptr<Label> head;
        std::shared_ptr<LabelList> tail;
        LabelList(std::shared_ptr<Label> headIn, std::shared_ptr<LabelList> tailIn) : head(headIn), tail(tailIn){}
};