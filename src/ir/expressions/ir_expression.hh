#pragma once
#include "../ir_node.hh"

class IRExpression : public IRNode
{
    public:
        virtual ~IRExpression() = 0;
};