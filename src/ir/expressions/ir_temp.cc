#include "ir_temp.hh"

IRTemp::IRTemp(std::shared_ptr<Temp> t) : temp(std::move(t))
{

}

std::string IRTemp::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;
    
    return "node" + std::to_string(myId) + " [label=\"TEMP: " + temp->ToString() + "\"];\n";
};