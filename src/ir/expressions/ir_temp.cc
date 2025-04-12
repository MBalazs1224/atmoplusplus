#include "ir_temp.hh"

IRTemp::IRTemp(std::shared_ptr<Temp> t) : temp(std::move(t))
{

}

std::string IRTemp::ToDotFormat(int& nodeCounter)
{
    int myId = nodeCounter++;
    
    return "node" + std::to_string(myId) + " [label=\"TEMP: " + temp->ToString() + "\"];\n";
};

std::shared_ptr<IRExpression> IRTemp::Build(std::shared_ptr<IRExpressionList> kids)
{
    return shared_from_this();
}

std::shared_ptr<IRExpressionList> IRTemp::Kids()
{
    return nullptr;
}