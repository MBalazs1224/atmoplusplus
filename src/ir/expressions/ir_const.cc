#include "ir_const.hh"

IRConst::IRConst(int v) : value(v)
{

}

std::string IRConst::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;
    
    return "node" + std::to_string(myId) + " [label=\"CONST: " + std::to_string(value) + "\"];\n";
}