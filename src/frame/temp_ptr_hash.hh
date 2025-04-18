#pragma once
#include <memory>
#include <functional>
#include "temp.hh"

// Clarifies how to has a shared_ptr to a Temp object. (required so that unordered_map can decide if an element is already in it)

struct TempPtrHash {
    std::size_t operator()(const std::shared_ptr<Temp>& t) const {
        return std::hash<int>()(t->num);
    }
};