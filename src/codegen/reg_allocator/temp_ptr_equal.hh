#pragma once
#include <memory>
#include <functional>
#include "../../frame/temp.hh"


struct TempPtrEqual {
    bool operator()(const std::shared_ptr<Temp>& lhs, const std::shared_ptr<Temp>& rhs) const {
        return lhs->num == rhs->num;
    }
};