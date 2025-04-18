#pragma once
#include <memory>
#include "temp.hh"
// Clarifies which temp pointers are actuall the samew (required so that unordered_map can decide if an element is already in it)

struct TempPtrEqual {
    bool operator()(const std::shared_ptr<Temp>& lhs, const std::shared_ptr<Temp>& rhs) const {
        return lhs->num == rhs->num;
    }
};