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
};