#pragma once
#include "../ir/expressions/ir_expression.hh"
#include <memory>
// TODO: Will be implemented in the future

class Access
{
    public:
        


        /*
        @param framePointer - The expression that yields the frame pointer.
        @return The expression that yields this location as it's value.
        */
        virtual std::shared_ptr<IRExpression> AsExpression(std::shared_ptr<IRExpression> framePointer) = 0;
};