#include "global_strings.hh"

std::unordered_map<std::string, std::shared_ptr<Label>> GlobalStrings::stringToLabel;

std::shared_ptr<Label> GlobalStrings::AddToPool(const std::string s)
{
    // THe string is already referenced
    if(stringToLabel.count(s) > 0)
    {
        return stringToLabel[s];
    }

    // Create a new label for the string and return the label
    
    auto label = std::make_shared<Label>();
    stringToLabel[s] = label;

    return label;
}