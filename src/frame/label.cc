#include "label.hh"


int Label::label_counter = 0;

Label::Label()
{
    name = "L" + std::to_string(label_counter++);
}

Label::Label(const std::string& name_in)
{
    name = name_in;
}

std::string Label::ToString()
{
    return name;
}