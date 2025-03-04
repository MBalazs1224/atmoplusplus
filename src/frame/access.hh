#pragma once
#include "../ir/expressions/ir_expression.hh"
#include <memory>
// TODO: Will be implemented in the future

class Access
{
    public:
        


        /*
        @param startOffset - The expression that yields the the location where the offset should start from if needed.
        @return The expression that yields this location as it's value.
        */
        virtual std::shared_ptr<IRExpression> AsExpression(std::shared_ptr<IRExpression> startOffset) = 0;

        /// @brief Will calculate the correct location of the resource, but won't dereference it.
        /// @param startOffset The expression that yields the the location where the offset should start from if needed.
        /// @return The expression that yields this location as it's value.
        virtual std::shared_ptr<IRExpression> AsExpressionNoDereference(std::shared_ptr<IRExpression> startOffset);
};