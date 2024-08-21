#pragma once
#include "node.hh"
#include "../../symboltable/symbols/symbolvariable.hh"
#include "../attributes/attribute.hh"
#include "../types/type.hh"
#include <memory>
#include <vector>

class FunctionDefinitionNode : public Node
{
    std::shared_ptr<Attribute> attribute;
    Type return_type;
    std::vector<std::shared_ptr<VariableSymbol>> arguments;

    public:
        FunctionDefinitionNode( std::shared_ptr<Attribute> attr_in,Type type_in,std::vector<std::shared_ptr<VariableSymbol>> args_in,yy::location loc)
        :  Node(loc), attribute(attr_in),return_type(type_in), arguments(std::move(args_in))
        {

        }
};