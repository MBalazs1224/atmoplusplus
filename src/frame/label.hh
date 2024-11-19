#pragma once
#include <string>
class Label
{
    private:
        std::string name;
        static int label_counter;

    public:
        // Creates a new label with a unique name
        Label();

        // Converts the label to a string
        std::string ToString();
        
        // Creates a new label with a given name
        Label(const std::string&);

};