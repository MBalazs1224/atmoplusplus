#include "ir_const_float.hh"

IRConstFloat::IRConstFloat(double in) : value(in)
{
}

std::string IRConstFloat::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;
    
    return "node" + std::to_string(myId) + " [label=\"CONST_FLOAT: " + std::to_string(value) + "\"];\n";
}

std::shared_ptr<IRExpression> IRConstFloat::Build(std::shared_ptr<IRExpressionList> kids)
{
    return shared_from_this();
}

std::shared_ptr<IRExpressionList> IRConstFloat::Kids()
{
    return nullptr;
}

DataSize IRConstFloat::Size()
{
    return DataSize::DWord; // 4 bytes
}