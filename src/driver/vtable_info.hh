#pragma once
#include <string>
#include <vector>
// Represents a vtable for a user defined class
class VtableInfo
{
    public:
        // Holds the label for the function pointers inside the vtable
        std::vector<std::string> functionPointers;

        // Represents the class's name
        std::string className;

        // If a vtable is empty then we don't need to print it to the final assembly
        bool IsEmpty()
        {
            return functionPointers.empty();
        }

        VtableInfo(std::string classn)
        {
            this->className = classn;
        }

};