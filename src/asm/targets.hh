#pragma once
#include <memory>
#include "../frame/labellist.hh"
class Targets
{
    public:
        std::shared_ptr<LabelList> head;
        Targets(std::shared_ptr<LabelList> headIn) : head(headIn) {}
};