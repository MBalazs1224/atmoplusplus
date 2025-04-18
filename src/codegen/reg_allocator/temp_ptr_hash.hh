#pragma once
#include <memory>
#include <functional>
#include "../../frame/temp.hh"
struct TempPtrHash {
    std::size_t operator()(const std::shared_ptr<Temp>& t) const {
        return std::hash<int>()(t->num);
    }
};