#include "ir_const.hh"

IRConst::IRConst(int v) : value(v)
{

}

std::string IRConst::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;
    
    return "node" + std::to_string(myId) + " [label=\"CONST: " + std::to_string(value) + "\"];\n";
}

std::shared_ptr<IRExpression> IRConst::Build(std::shared_ptr<IRExpressionList> kids)
{
    return shared_from_this();
}

std::shared_ptr<IRExpressionList> IRConst::Kids()
{
    return nullptr;
}