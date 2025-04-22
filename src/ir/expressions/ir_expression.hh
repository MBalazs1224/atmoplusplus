#pragma once
#include <memory>
#include "../ir_node.hh"
#include "../../frame/datasize.hh"

class IRExpressionList;

class IRExpression : public IRNode
{
    public:
        virtual ~IRExpression() = 0;
        virtual std::shared_ptr<IRExpression> Build (std::shared_ptr<IRExpressionList> kids) = 0;
        virtual std::shared_ptr<IRExpressionList> Kids () = 0;

        /// @return How much space this expression's result will take in bytes. 
        virtual DataSize Size() = 0;
};