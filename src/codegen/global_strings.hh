#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "../frame/label.hh"
class GlobalStrings
{
    public:
        // Maps strings to their generated labels
        static std::unordered_map<std::string, std::shared_ptr<Label>> stringToLabel;

        // Will add the needed string to the pool if it's not already contained and generates a new label for it if needed. Will return the used label,
        static std::shared_ptr<Label> AddToPool(const std::string);
};