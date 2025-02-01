#include "ir_const_float.hh"

IRConstFloat::IRConstFloat(double in) : value(in)
{
}

std::string IRConstFloat::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;
    
    return "node" + std::to_string(myId) + " [label=\"CONST_FLOAT: " + std::to_string(value) + "\"];\n";
}