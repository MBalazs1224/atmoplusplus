#pragma once
#include "body_node.hh"
#include "node.hh"
#include "../expressions/expressionable.hh"
#include "../attributes/attribute.hh"
#include "../types/type.hh"
#include <memory>
#include <iostream>

class VariableDefinitionNode : public Node
{
    std::shared_ptr<Attribute> attribute;
    Type type;
    std::shared_ptr<IExpressionable> init_exp;

    public:
        VariableDefinitionNode( std::shared_ptr<Attribute> attr_in,Type type_in,std::shared_ptr<IExpressionable> init_exp_in,yy::location loc)
        :  Node(loc), attribute(attr_in),type(type_in), init_exp(init_exp_in)
        {

        }
};